#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>

#include "Config.h"
#include "Shader.h"
#include "Model.h"

std::unique_ptr<Scene> Scene::loadScene(const std::string& fileName)
{
	return std::make_unique<Scene3D>();
}


Scene3D::Scene3D() :
    m_light(glm::vec3(1.0f, 1.0f, 1.0f), // white light
        glm::vec3(1.0f, -1.0f, 1.0f), // coming from above
        0.3f, 0.7f),// full ambient and diffuse intensity
    m_camera(glm::vec3(1.5f, 2.5f, 3.5f), // initial location
        glm::vec3(0.0f, 1.0f, 0.0f), // up direction
        -110.0f, // pitch: rotation in XY plane from the X axis
        -20.0f, // yaw: rotation up-down from the horizontal
        10.0f,  // linear move speed pixel/sec
        0.05f),// rotational move speed a.k.a. mouse sensitivity
    m_models(),
    m_instances(),
    m_shaders()
{
    m_shaders.push_back(std::make_unique<Shader>(
        SHADERS_DIR + "vertexShader_texture.glsl",
        SHADERS_DIR + "fragmentShader_texture.glsl"));
    m_shaders.push_back(std::make_unique<Shader>(
        SHADERS_DIR + "vertexShader_noTexture.glsl",
        SHADERS_DIR + "fragmentShader_noTexture.glsl"));

    try
    {
        m_models["sphere"] = make_unique<Model>("sphere");
        m_instances.push_back(make_unique<ModelInstance>(m_models["sphere"].get(),
            0.0f, 1.0f, 0.0f));
    }
    catch (const std::exception& e)
    {
        debugOutput(e.what());
    }

    try
    {
        m_models["floor"] = make_unique<Model>("floor");
        m_instances.push_back(make_unique<ModelInstance>(m_models["floor"].get(),
            0.0f, 0.0f, 0.0f, 3.0f));
    }
    catch (const std::exception& e)
    {
        debugOutput(e.what());
    }
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
        it->render(shader);

    // deactivate/unbind shader
    glUseProgram(0);
}

Scene3D::~Scene3D() {}