#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "StaticModel.h"
#include "Shader.h"
#include "Camera.h"
#include "Config.h"
#include "Texture.h"
#include "Light.h"
#include "Primitive.h"

#include <assimp/Importer.hpp>

int main() {

    // Window dimensions
    const GLint WIDTH = 800, HEIGHT = 600;
    // create & initialize main window
    Window mainWindow(WIDTH,HEIGHT);
    auto out = mainWindow.initialize();
    if (!out.ok())
    {
        std::cout << out.message() << std::endl;
        return 1;
    }

    Assimp::Importer importer;
    
    // create meshes
    auto plane = makePlane();
    auto cube = makeCube();
    auto pyramid = makePyramid();
    
    // create textures
    Texture grass(TEXTURES_DIR + "/grass.png");
    Texture brick(TEXTURES_DIR + "/brick.png");
    Texture straw(TEXTURES_DIR + "/straw.png");
    
    // container with pointers to models to draw
    std::vector<std::unique_ptr<StaticModel> > models;
    
    // create floor
    glm::mat4 modelMatrix(1.0f); // "base" transformation matrix
    models.push_back(std::make_unique<StaticModel>(plane.get(),&grass,
                     glm::scale(modelMatrix, glm::vec3(10.0,10.0,10.0))));
    
    // create walls; first translate, then scale
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0,2.0,2.0));
    models.push_back(std::make_unique<StaticModel>(cube.get(),&brick,
                     glm::translate(modelMatrix, glm::vec3(0.0f,0.5f,0.0f))));
    
    // create roof
    modelMatrix = glm::mat4(1.0f); // reset base matrix to identity
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0,2.0,2.0));
    models.push_back(std::make_unique<StaticModel>(pyramid.get(),&straw,
                     glm::translate(modelMatrix, glm::vec3(0.0f,1.0f,0.0f))));
    
    // create another "house"
    modelMatrix = glm::mat4(1.0f); // reset base matrix to identity
    models.push_back(std::make_unique<StaticModel>(cube.get(),&brick,
                     glm::translate(modelMatrix, glm::vec3(2.0f,0.5f,-2.0f))));
    models.push_back(std::make_unique<StaticModel>(pyramid.get(),&straw,
                     glm::translate(modelMatrix, glm::vec3(2.0f,1.0f,-2.0f))));
    
    // create a light object
    Light light(glm::vec3(1.0f,1.0f,1.0f), // white light
                glm::vec3(1.0f,-1.0f,1.0f), // comming from above
                0.3f, 0.7f); // full ambient and diffuse intensity

    // create a camera object
    Camera camera(glm::vec3(1.5f,2.5f,3.5f), // initial location
                  glm::vec3(0.0f,1.0f,0.0f), // up direction
                  -110.0f, // pitch: rotation in XY plane from the X axis
                  -20.0f, // yaw: rotation up-down from the horizontal
                  10.0f,  // linear move speed pixel/sec
                  0.05f); // rotational move speed a.k.a. mouse sensitivity

    // create and compile shaders on GPU
    Shader shaderTexture(SHADERS_DIR + "/vertexShader_texture.glsl",SHADERS_DIR + "/fragmentShader_texture.glsl");
    Shader shaderNoTexture(SHADERS_DIR + "/vertexShader_noTexture.glsl", SHADERS_DIR + "/fragmentShader_noTexture.glsl");
    
    // projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth()/(GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);
    
    // shader switching; will move to a dedicated class
    bool useTextures = true;

    // Loop until window closed
    while (!mainWindow.shouldClose())
    {
        // Get + Handle user input events
        glfwPollEvents();

        // pass data to the camera
        camera.keyControl(mainWindow.getKeys(),mainWindow.events().timeStep());
        camera.mouseControl(mainWindow.getChangeX(), mainWindow.getChangeY());
        
        // shader switching
        if (mainWindow.getKeys()[GLFW_KEY_G])
        {
            useTextures = !useTextures;
            mainWindow.getKeys()[GLFW_KEY_G] = false;
        }
        Shader & shader = useTextures ? shaderTexture : shaderNoTexture;
        
        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // activate/bind a shader to use it
        shader.useShader();
        
        // copy view and projection matrices to the GPU
        glUniformMatrix4fv(shader.uniViewMatrix(),1,GL_FALSE,
                           glm::value_ptr(camera.viewMatrix()));
        glUniformMatrix4fv(shader.uniProjMatrix(),1,GL_FALSE,
                           glm::value_ptr(projection));

        // activate Light
        light.useLight(shader.uniLightColor(), shader.uniLightDirection(), shader.uniAmbientIntensity(), shader.uniDiffuseIntensity());
        
        
        glUniform3f(shader.uniCameraPosition(), camera.cameraPosition().x, camera.cameraPosition().y, camera.cameraPosition().z);
        
        glUniform1f(shader.uniMaterialShininess(), 32);
        glUniform1f(shader.uniSpecularIntensity(),4.0);
        
        // render models
        for (auto &it : models)
            it->render(shader);

        // deactivate/unbind shader
        glUseProgram(0);

        // refresh the screen buffer (actual draw)
        mainWindow.swapBuffers();
    }

    return 0;
}
