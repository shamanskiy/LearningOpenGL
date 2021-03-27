#include "Scene.h"

#include <fstream>
#include <vector>
#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>
#include <json.hpp>

#include "Config.h"
#include "Shader.h"
#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "Utils.h"

namespace
{
    class Scene3D : public Scene
    {
    public:
        Scene3D() = default;
        Scene3D(const nlohmann::json& sceneJson);

        // Get fresh events and render the scene
        void render(const EventContainer& events) override;

    private:
        void loadJsonModels(const nlohmann::json& sceneJson);
        void loadJsonCamera(const nlohmann::json& sceneJson);
        void loadJsonLight(const nlohmann::json& sceneJson);
        void loadJsonInstances(const nlohmann::json& sceneJson);

    private:
        // Interactive camera
        Camera m_camera;
        // List of shaders that can be used to render the scene.
        // Can be cycled through.
        Shader m_shader;

        // List of Models used in the Scene
        unordered_map<string, Model> m_models;
        // Instances of Models that are scaled and translated to positions
        vector<ModelInstance> m_instances;

        // Ambient + directional light
        Light m_light;
    };
}

std::unique_ptr<Scene> Scene::loadScene(const std::string& fileName)
{
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open())
        throw std::runtime_error("Failed to open " + fileName);
    nlohmann::json sceneJson;
    inputFile >> sceneJson;
    inputFile.close();

    if (sceneJson["sceneType"].get<std::string>() == "3D")
        return std::make_unique<Scene3D>(sceneJson);
    else
        return std::make_unique<Scene3D>();
}

Scene::~Scene() = default;

Scene3D::Scene3D(const nlohmann::json& sceneJson)
{
    loadJsonModels(sceneJson);
    loadJsonInstances(sceneJson);
    loadJsonCamera(sceneJson);
    loadJsonLight(sceneJson);
}

void Scene3D::loadJsonModels(const nlohmann::json& sceneJson)
{
    for (auto& model : sceneJson["models"])
        try
        {
            m_models[model] = Model(model);
        }
        catch (const ModelException& e)
        {
            debugOutput(e.what());
        }
}

void Scene3D::loadJsonInstances(const nlohmann::json& sceneJson)
{
    for (auto& instance : sceneJson["instances"])
        if (m_models.find(instance["modelName"]) != m_models.end())
            m_instances.push_back(
                ModelInstance(
                    m_models[instance["modelName"]],
                    instance["origin"][0],
                    instance["origin"][1],
                    instance["origin"][2],
                    instance["scale"]));
}

void Scene3D::loadJsonCamera(const nlohmann::json& sceneJson)
{
    m_camera = Camera(
        glm::vec3(
            sceneJson["camera"]["origin"][0],
            sceneJson["camera"]["origin"][1],
            sceneJson["camera"]["origin"][2]),
        sceneJson["camera"]["pitch"],
        sceneJson["camera"]["yaw"],
        sceneJson["camera"]["move_speed"],
        sceneJson["camera"]["rotation_speed"]
    );
}

void Scene3D::loadJsonLight(const nlohmann::json& sceneJson)
{
    m_light = Light(
        glm::vec3(
            sceneJson["light"]["color"][0],
            sceneJson["light"]["color"][1],
            sceneJson["light"]["color"][2]),
        glm::vec3(
            sceneJson["light"]["direction"][0],
            sceneJson["light"]["direction"][1],
            sceneJson["light"]["direction"][2]),
        sceneJson["light"]["ambientIntensity"],
        sceneJson["light"]["diffuseIntensity"]
    );
}

void Scene3D::render(const EventContainer& events)
{
    // projection matrix
    glm::mat4 projection = glm::perspective(45.0f, events.aspectRatio(), 0.1f, 100.0f);

    // pass data to the camera
    m_camera.processEvents(events);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate/bind a shader to use it
    m_shader.activateShader();

    // copy view and projection matrices to the GPU
    glUniformMatrix4fv(m_shader.uniforms().viewMatrix, 1, GL_FALSE,
        glm::value_ptr(m_camera.viewMatrix()));
    glUniformMatrix4fv(m_shader.uniforms().projectionMatrix, 1, GL_FALSE,
        glm::value_ptr(projection));

    // activate Light
    m_light.useLight(
        m_shader.uniforms().lightColor,
        m_shader.uniforms().lightDirection,
        m_shader.uniforms().ambientIntensity,
        m_shader.uniforms().diffuseIntensity);


    glUniform3f(
        m_shader.uniforms().cameraPosition,
        m_camera.position().x,
        m_camera.position().y,
        m_camera.position().z);

    glUniform1f(m_shader.uniforms().materialShininess, 32);
    glUniform1f(m_shader.uniforms().specularIntensity, 4.0);

    for (auto& it : m_instances)
        it.render(m_shader);
}