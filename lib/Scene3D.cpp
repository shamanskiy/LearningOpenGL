#include "Scene3D.h"

#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Primitive.h"
#include "Texture.h"
#include "StaticModel.h"
#include "Config.h"
#include "Shader.h"

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
    meshes(),
    textures(),
    models(),
    shaders(),
    shaderSwitcher(false),
    model()
{
    meshes.push_back(makePlane());
    meshes.push_back(makeCube());
    meshes.push_back(makePyramid());

    textures.push_back(std::make_unique<Texture>(TEXTURES_DIR + "grass.png"));
    textures.push_back(std::make_unique<Texture>(TEXTURES_DIR + "brick.png"));
    textures.push_back(std::make_unique<Texture>(TEXTURES_DIR + "straw.png"));

    // create floor
    glm::mat4 modelMatrix(1.0f); // "base" transformation matrix
    models.push_back(std::make_unique<StaticModel>(meshes[0].get(), textures[0].get(),
        glm::scale(modelMatrix, glm::vec3(10.0, 10.0, 10.0))));

    // create walls; first translate, then scale
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0, 2.0, 2.0));
    models.push_back(std::make_unique<StaticModel>(meshes[1].get(), textures[1].get(),
        glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f))));

    // create roof
    modelMatrix = glm::mat4(1.0f); // reset base matrix to identity
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0, 2.0, 2.0));
    models.push_back(std::make_unique<StaticModel>(meshes[2].get(), textures[2].get(),
        glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f))));

    // create another "house"
    modelMatrix = glm::mat4(1.0f); // reset base matrix to identity
    models.push_back(std::make_unique<StaticModel>(meshes[1].get(), textures[1].get(),
        glm::translate(modelMatrix, glm::vec3(2.0f, 0.5f, -2.0f))));
    models.push_back(std::make_unique<StaticModel>(meshes[2].get(), textures[2].get(),
        glm::translate(modelMatrix, glm::vec3(2.0f, 1.0f, -2.0f))));

    shaders.push_back(std::make_unique<Shader>(
        SHADERS_DIR + "vertexShader_texture.glsl",
        SHADERS_DIR + "fragmentShader_texture.glsl"));
    shaders.push_back(std::make_unique<Shader>(
        SHADERS_DIR + "vertexShader_noTexture.glsl",
        SHADERS_DIR + "fragmentShader_noTexture.glsl"));

    model.loadModel(MODELS_DIR + "table.obj");

}

void Scene3D::render(const EventContainer& events)
{
    // projection matrix
    glm::mat4 projection = glm::perspective(45.0f, events.aspectRatio(), 0.1f, 100.0f);

    // pass data to the camera
    camera.keyControl(events);
    camera.mouseControl(events.cursorPositionChangeX(),
        events.cursorPositionChangeY());

    // shader switching
    if (events.keyState(GLFW_KEY_G))
    {
        shaderSwitcher = !shaderSwitcher;
    }
    Shader& shader = shaderSwitcher ? *shaders[0] : *shaders[1];

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate/bind a shader to use it
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

    // render models
    for (auto& it : models)
        it->render(shader);

    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::scale(modelMat, glm::vec3(0.1f, 0.1f, 0.1f));
    modelMat = glm::translate(modelMat, glm::vec3(50.0f, -35.0f, 84.0f));
    //modelMat = glm::scale(modelMat, glm::vec3(0.01f, 0.01f, 0.01f));
    glUniformMatrix4fv(shader.uniModelMatrix(), 1, GL_FALSE, glm::value_ptr(modelMat));
    model.renderModel();

    // deactivate/unbind shader
    glUseProgram(0);
}

Scene3D::~Scene3D() {}