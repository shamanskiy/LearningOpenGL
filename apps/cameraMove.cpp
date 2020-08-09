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

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// container with objects to draw
std::vector<Mesh *> objects;

// vertex shader filename
std::string vShader = "../Shaders/vShader.txt";
// fragment shader filename
std::string fShader = "../Shaders/fShader.txt";

void CreateObjects()
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
    
    // create & initialize main window
    Window mainWindow(WIDTH,HEIGHT);
    if (mainWindow.initialize() != 0)
    {
        std::cout << "Problem creating window!\n";
        return 1;
    }
    
    // create objects on GPU
    CreateObjects();
    
    // create camera object
    Camera camera(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f),
                  -90.0f,0.0f,10.0f,0.05f);
    
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
        
        shader.useShader();
        uniModel = shader.getUniformModel();
        uniView = shader.getUniformView();
        uniProjection = shader.getUniformProjection();
        glUniformMatrix4fv(uniProjection,1,GL_FALSE,glm::value_ptr(projection));
        glUniformMatrix4fv(uniView,1,GL_FALSE,glm::value_ptr(camera.viewMatrix()));
        
        
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,0.0f,-5.0f));
        //model = glm::scale(model,glm::vec3(0.5f,0.5f,1.0f));
        glUniformMatrix4fv(uniModel,1,GL_FALSE,glm::value_ptr(model));
        objects[0]->renderMesh();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f,2.0f,-5.0f));
        //model = glm::scale(model,glm::vec3(0.5f,0.5f,1.0f));
        glUniformMatrix4fv(uniModel,1,GL_FALSE,glm::value_ptr(model));
        objects[0]->renderMesh();
        
        glUseProgram(0);

        mainWindow.swapBuffers();
    }
    
    delete objects[0];
    
    
    return 0;
}
