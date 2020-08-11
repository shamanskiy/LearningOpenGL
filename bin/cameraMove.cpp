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

void CreateObjects(std::vector<Mesh *> & objects)
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, -1.0f,
        0.0f, 2.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };

    unsigned int elements[] = {
        0,3,1,
        1,3,2,
        2,3,0,
        0,1,2
    };

    Mesh * obj1 = new Mesh();
    obj1->createMesh(vertices,elements,12,12);
    objects.push_back(obj1);
}

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
    std::vector<Mesh *> objects;
    // create objects on GPU and save pointers to objects
    CreateObjects(objects);

    // create acamera object
    Camera camera(glm::vec3(0.0f,0.0f,0.0f), // centered at the origin
                  glm::vec3(0.0f,1.0f,0.0f), // global up direction
                  -90.0f,                    // initial pitch -> along -Z axis
                  0.0f,                      // initial yaw -> strictly horizontal
                  10.0f,                     // linear move speed pixel/sec
                  0.05f);                    // rotational move speed a.k.a. mouse sensitivity

    // vertex shader filename
    std::string vShader = std::string(LEARNING_OPENGL_SHADERS_DIR) + "/vShader.txt";
    // fragment shader filename
    std::string fShader = std::string(LEARNING_OPENGL_SHADERS_DIR) + "/fShader.txt";
    // create and compile shaders on GPU
    Shader shader;
    shader.createFromFile(vShader, fShader);
    GLuint uniModel, uniView, uniProjection;

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
        // get locations of uniform variables on the GPU
        uniModel = shader.getUniformModel();
        uniView = shader.getUniformView();
        uniProjection = shader.getUniformProjection();
        // copy view and projection matrices to the GPU
        glUniformMatrix4fv(uniView,1,GL_FALSE,glm::value_ptr(camera.viewMatrix()));
        glUniformMatrix4fv(uniProjection,1,GL_FALSE,glm::value_ptr(projection));

        // set model matrix for the first object and copy in to the GPU
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,0.0f,-5.0f));
        glUniformMatrix4fv(uniModel,1,GL_FALSE,glm::value_ptr(model));
        objects[0]->renderMesh();

        // set model matrix for the second object and copy in to the GPU
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f,2.0f,-5.0f));
        glUniformMatrix4fv(uniModel,1,GL_FALSE,glm::value_ptr(model));
        objects[0]->renderMesh();

        // deactivate/unbind shader
        glUseProgram(0);

        // refresh the screen buffer (actual draw)
        mainWindow.swapBuffers();
    }

    delete objects[0];
    delete objects[1];

    return 0;
}
