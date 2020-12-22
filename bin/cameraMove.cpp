// some funny preprocessor command to enable stb_image library
#define STB_IMAGE_IMPLEMENTATION

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
#include "Shader.h"
#include "Camera.h"
#include "Config.h"
#include "Texture.h"
#include "Light.h"
#include "Primitive.h"

int main() {

    // Window dimensions
    const GLint WIDTH = 800, HEIGHT = 600;
    // create & initialize main window
    Window mainWindow(WIDTH,HEIGHT);
    if (mainWindow.initialize() != 0)
    {
        std::cout << "Problem creating window!\n";
        return 1;
    }

    // container with pointers to objects to draw
    std::vector<std::unique_ptr<Mesh> > objects;
    // create objects on GPU and save pointers to objects
    objects.push_back(std::move(makePlane()));
    objects.push_back(std::move(makeCube()));
    objects.push_back(std::move(makePyramid()));

    // create texture objects
    Texture grass(std::string(LEARNING_OPENGL_SOURCE_PATH) + "/textures/grass.png");
    grass.loadTexture();
    Texture brick(std::string(LEARNING_OPENGL_SOURCE_PATH) + "/textures/brick.png");
    brick.loadTexture();
    Texture straw(std::string(LEARNING_OPENGL_SOURCE_PATH) + "/textures/straw.png");
    straw.loadTexture();

    // create a light object
    Light light(glm::vec3(1.0f,1.0f,1.0f), // white light
                glm::vec3(-1.0f,-0.0f,-0.0f), // comming from above
                1.0f, 3.0f); // full ambient and diffuse intensity

    // create a camera object
    Camera camera(glm::vec3(1.5f,2.5f,3.5f), // initial location
                  glm::vec3(0.0f,1.0f,0.0f), // up direction
                  -110.0f, // pitch: rotation in XY plane from the X axis
                  -20.0f, // yaw: rotation up-down from the horizontal
                  10.0f,  // linear move speed pixel/sec
                  0.05f); // rotational move speed a.k.a. mouse sensitivity

    // create and compile shaders on GPU
    Shader shader;
    shader.createFromFile(std::string(LEARNING_OPENGL_SOURCE_PATH) + "/src/shaders/vShader.glsl",
                         std::string(LEARNING_OPENGL_SOURCE_PATH) +
        "/src/shaders/fShader.glsl");
    
    // projection matrix
    glm::mat4 projection(1.0f);
    projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth()/(GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // time tracking for delta t
    GLfloat lastTime = glfwGetTime();

    // Loop until window closed
    while (!mainWindow.shouldClose())
    {
        // Get + Handle user input events
        glfwPollEvents();

        // compute delta t
        GLfloat timeNow = glfwGetTime();
        GLfloat delta_t = timeNow - lastTime;
        lastTime = timeNow;

        // pass data to the camera
        camera.keyControl(mainWindow.getKeys(),delta_t);
        camera.mouseControl(mainWindow.getChangeX(), mainWindow.getChangeY());

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

        // set model matrix for the floor and copy in to the GPU
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(10.0,10.0,10.0));
        //model = glm::translate(model, glm::vec3(-0.5f,0.0f,-0.5f));
        glUniformMatrix4fv(shader.uniModelMatrix(),1,GL_FALSE,
                           glm::value_ptr(model));
        // activate texture
        grass.useTexture();
        // render the floor
        objects[0]->renderMesh();

        // set model matrix for the cube and copy in to the GPU
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(2.0,2.0,2.0));
        model = glm::translate(model, glm::vec3(0.0f,0.5f,0.0f));
        glUniformMatrix4fv(shader.uniModelMatrix(),1,GL_FALSE,
                           glm::value_ptr(model));
        // activate texture
        brick.useTexture();
        // render the cube
        objects[1]->renderMesh();

        // set model matrix for the pyramid and copy in to the GPU
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(2.0,2.0,2.0));
        model = glm::translate(model, glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(shader.uniModelMatrix(),1,GL_FALSE,
                           glm::value_ptr(model));
        // activate texture
        straw.useTexture();
        // render the cube
        objects[2]->renderMesh();

        // deactivate/unbind shader
        glUseProgram(0);

        // refresh the screen buffer (actual draw)
        mainWindow.swapBuffers();
    }

    return 0;
}
