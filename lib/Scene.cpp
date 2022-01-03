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
        void loadModels(const nlohmann::json& sceneJson);
        void loadCamera(const nlohmann::json& sceneJson);
        void loadLight(const nlohmann::json& sceneJson);
        void loadInstances(const nlohmann::json& sceneJson);
        void loadBackgroundColor(const nlohmann::json& sceneJson);

        void resetFrame(const EventContainer& events) const;

    private:
        Shader m_shader;

        Camera m_camera;
        unordered_map<string, Model> m_models;
        vector<ModelInstance> m_instances;
        LightManager m_lights;
        glm::vec3 m_backgroundColor;
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

void Scene3D::render(const EventContainer& events)
{
    m_shader.activateShader();

    resetFrame(events);
    m_camera.processEvents(events);
    m_lights.processEvents(events);
    
    m_camera.talkToShader(m_shader.id());
    m_lights.talkToShader(m_shader.id());

    for (auto& it : m_instances)
        it.render(m_shader);
}

Scene3D::Scene3D(const nlohmann::json& sceneJson)
{
    loadModels(sceneJson);
    loadInstances(sceneJson);
    loadCamera(sceneJson);
    loadLight(sceneJson);
    loadBackgroundColor(sceneJson);
}

void Scene3D::loadBackgroundColor(const nlohmann::json& sceneJson)
{
    m_backgroundColor = glm::vec3(
        sceneJson["backgroundColor"][0],
        sceneJson["backgroundColor"][1],
        sceneJson["backgroundColor"][2]
    );
}

void Scene3D::loadModels(const nlohmann::json& sceneJson)
{
    for (auto& model : sceneJson["models"])
        try
        {
            m_models[model] = Model(model);
            debugOutput(m_models[model].boundingBoxAsString());
        }
        catch (const ModelException& e)
        {
            debugOutput(e.what());
        }
}

void Scene3D::loadInstances(const nlohmann::json& sceneJson)
{
    for (auto& instance : sceneJson["instances"])
        if (m_models.find(instance["model"]) != m_models.end())
            m_instances.push_back(
                ModelInstance(
                    m_models[instance["model"]],
                    instance["origin"][0],
                    instance["origin"][1],
                    instance["origin"][2],
                    instance["scale"]));
}

void Scene3D::loadCamera(const nlohmann::json& sceneJson)
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

void Scene3D::loadLight(const nlohmann::json& sceneJson)
{
    for (auto & light : sceneJson["lights"])
        if (light["type"] == "ambient")
        {
            m_lights.setAmbientLight(AmbientLight(
                glm::vec3(
                    light["color"][0],
                    light["color"][1],
                    light["color"][2]),
                light["intensity"]
                ));
        }
        else if (light["type"] == "directional")
        {
            m_lights.setDirectionalLight(DirectionalLight(
                glm::vec3(
                    light["color"][0],
                    light["color"][1],
                    light["color"][2]),
                glm::vec3(
                    light["direction"][0],
                    light["direction"][1],
                    light["direction"][2]),
                light["intensity"]
                ));
        }
        else if (light["type"] == "point")
        {
            m_lights.addPointLight(PointLight(
                glm::vec3(
                    light["color"][0],
                    light["color"][1],
                    light["color"][2]),
                glm::vec3(
                    light["position"][0],
                    light["position"][1],
                    light["position"][2]),
                glm::vec3(
                    light["attenuation"][0],
                    light["attenuation"][1],
                    light["attenuation"][2]),
                light["intensity"]
                ));
        }
        else if (light["type"] == "spot")
        {
            m_lights.setSpotLight(SpotLight(
                glm::vec3(
                    light["color"][0],
                    light["color"][1],
                    light["color"][2]),
                glm::vec3(
                    light["attenuation"][0],
                    light["attenuation"][1],
                    light["attenuation"][2]),
                light["intensity"],
                light["halfAngle"],
                light["verticalOffset"],
                light["isOn"] == 0 ? false : true
            ));
        }
}

void Scene3D::resetFrame(const EventContainer& events) const
{
    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z,
        1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(45.0f, events.aspectRatio(), 0.1f, 100.0f);
    glUniformMatrix4fv(m_shader.uniforms().projectionMatrix, 1, GL_FALSE,
        glm::value_ptr(projection));
}
