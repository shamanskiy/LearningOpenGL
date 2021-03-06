#include "Scene3D.h"

#include <glm/gtc/type_ptr.hpp>

#include "Config.h"
#include "Shader.h"
#include "Model.h"

Scene3D::Scene3D() :
    light(glm::vec3(1.0f, 1.0f, 1.0f), // white light
        glm::vec3(1.0f, -1.0f, 1.0f), // coming from above
        0.3f, 0.7f),// full ambient and diffuse intensity
    camera(glm::vec3(1.5f, 2.5f, 3.5f), // initial location
        glm::vec3(0.0f, 1.0f, 0.0f), // up direction
        -110.0f, // pitch: rotation in XY plane from the X axis
        -20.0f, // yaw: rotation up-down from the horizontal
        10.0f,  // linear move speed pixel/sec
        0.05f),// rotational move speed a.k.a. mouse sensitivity
    models(),
    shaders()
{
    shaders.push_back(std::make_unique<Shader>(
        SHADERS_DIR + "vertexShader_texture.glsl",
        SHADERS_DIR + "fragmentShader_texture.glsl"));
    shaders.push_back(std::make_unique<Shader>(
        SHADERS_DIR + "vertexShader_noTexture.glsl",
        SHADERS_DIR + "fragmentShader_noTexture.glsl"));

    models.push_back(make_unique<Model>());
    models[0]->loadModel(MODELS_DIR + "sphere.obj");
    models.push_back(make_unique<Model>());
    models[1]->loadModel(MODELS_DIR + "floor.obj");
}

void Scene3D::render(const EventContainer& events)
{
    // projection matrix
    glm::mat4 projection = glm::perspective(45.0f, events.aspectRatio(), 0.1f, 100.0f);

    // pass data to the camera
    camera.keyControl(events);
    camera.mouseControl(events.cursorPositionChangeX(),
        events.cursorPositionChangeY());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate/bind a shader to use it
    Shader& shader = *shaders[0];
    shader.useShader();

    // copy view and projection matrices to the GPU
    glUniformMatrix4fv(shader.uniViewMatrix(), 1, GL_FALSE,
        glm::value_ptr(camera.viewMatrix()));
    glUniformMatrix4fv(shader.uniProjMatrix(), 1, GL_FALSE,
        glm::value_ptr(projection));

    // activate Light
    light.useLight(shader.uniLightColor(), shader.uniLightDirection(), shader.uniAmbientIntensity(), shader.uniDiffuseIntensity());


    glUniform3f(shader.uniCameraPosition(), camera.cameraPosition().x, camera.cameraPosition().y, camera.cameraPosition().z);

    glUniform1f(shader.uniMaterialShininess(), 32);
    glUniform1f(shader.uniSpecularIntensity(), 4.0);

    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 1.0f, 0.0f));
    //modelMat = glm::scale(modelMat, glm::vec3(0.1f, 0.1f, 0.1f));
    //modelMat = glm::scale(modelMat, glm::vec3(0.01f, 0.01f, 0.01f));
    glUniformMatrix4fv(shader.uniModelMatrix(), 1, GL_FALSE, glm::value_ptr(modelMat));
    models[0]->renderModel();
    
    modelMat = glm::mat4(1.0f);
    modelMat = glm::scale(modelMat, glm::vec3(3.0f, 3.0f, 3.0f));
    glUniformMatrix4fv(shader.uniModelMatrix(), 1, GL_FALSE, glm::value_ptr(modelMat));
    models[1]->renderModel();

    // deactivate/unbind shader
    glUseProgram(0);
}

Scene3D::~Scene3D() {}