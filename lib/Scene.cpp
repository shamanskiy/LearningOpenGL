#include "Scene.h"

#include <fstream>
#include <iostream>
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
        Scene3D(const nlohmann::json& sceneJson);
        ~Scene3D() override;

        // Get fresh events and render the scene
        void render(const EventContainer& events) override;

    private:
        void loadJsonScene(const nlohmann::json& sceneJson);

    private:
        // Interactive camera
        Camera m_camera;
        // List of shaders that can be used to render the scene.
        // Can be cycled through.
        vector<unique_ptr<Shader> > m_shaders{};

        // List of Models used in the Scene
        unordered_map<string, Model> m_models{};
        // Instances of Models that are scaled and translated to positions
        vector<ModelInstance> m_instances{};

        // Ambient + directional light
        Light m_light;
    };
}

std::unique_ptr<Scene> Scene::loadScene(const std::string& fileName)
{
    std::ifstream inputFile(SCENES_DIR + "exampleScene.json");
    nlohmann::json sceneJson;
    inputFile >> sceneJson;

    if (sceneJson["sceneType"].get<std::string>() == "3D")
    {
        debugOutput("3D scene");
        return std::make_unique<Scene3D>(sceneJson);
    }
}

Scene3D::Scene3D(const nlohmann::json& sceneJson) :
    m_light(glm::vec3(1.0f, 1.0f, 1.0f), // white light
        glm::vec3(1.0f, -1.0f, 1.0f), // coming from above
        0.3f, 0.7f),// full ambient and diffuse intensity
    m_camera(glm::vec3(1.5f, 2.5f, 3.5f), // initial location
        glm::vec3(0.0f, 1.0f, 0.0f), // up direction
        -110.0f, // pitch: rotation in XY plane from the X axis
        -20.0f, // yaw: rotation up-down from the horizontal
        10.0f,  // linear move speed pixel/sec
        0.05f)// rotational move speed a.k.a. mouse sensitivity
{
    m_shaders.push_back(std::make_unique<Shader>(
        SHADERS_DIR + "vertexShader_texture.glsl",
        SHADERS_DIR + "fragmentShader_texture.glsl"));
    m_shaders.push_back(std::make_unique<Shader>(
        SHADERS_DIR + "vertexShader_noTexture.glsl",
        SHADERS_DIR + "fragmentShader_noTexture.glsl"));

    loadJsonScene(sceneJson);

    try
    {
        m_models["sphere"] = Model("sphere");
        m_instances.push_back(ModelInstance(m_models["sphere"],
            0.0f, 1.0f, 0.0f));
    }
    catch (const std::exception& e)
    {
        debugOutput(e.what());
    }

    try
    {
        m_models["floor"] = Model("floor");
        m_instances.push_back(ModelInstance(m_models["floor"],
            0.0f, 0.0f, 0.0f, 3.0f));
    }
    catch (const std::exception& e)
    {
        debugOutput(e.what());
    }
}

void Scene3D::loadJsonScene(const nlohmann::json& sceneJson)
{
    for (auto& el : sceneJson["models"])
        std::cout << el.get<std::string>();
}

void Scene3D::render(const EventContainer& events)
{
    // projection matrix
    glm::mat4 projection = glm::perspective(45.0f, events.aspectRatio(), 0.1f, 100.0f);

    // pass data to the camera
    m_camera.keyControl(events);
    m_camera.mouseControl(events.cursorPositionChangeX(),
        events.cursorPositionChangeY());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate/bind a shader to use it
    Shader& shader = *m_shaders[0];
    shader.useShader();

    // copy view and projection matrices to the GPU
    glUniformMatrix4fv(shader.uniViewMatrix(), 1, GL_FALSE,
        glm::value_ptr(m_camera.viewMatrix()));
    glUniformMatrix4fv(shader.uniProjMatrix(), 1, GL_FALSE,
        glm::value_ptr(projection));

    // activate Light
    m_light.useLight(shader.uniLightColor(), shader.uniLightDirection(), shader.uniAmbientIntensity(), shader.uniDiffuseIntensity());


    glUniform3f(shader.uniCameraPosition(), m_camera.cameraPosition().x, m_camera.cameraPosition().y, m_camera.cameraPosition().z);

    glUniform1f(shader.uniMaterialShininess(), 32);
    glUniform1f(shader.uniSpecularIntensity(), 4.0);

    for (auto& it : m_instances)
        it.render(shader);

    // deactivate/unbind shader
    glUseProgram(0);
}

Scene3D::~Scene3D() {}