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

Mesh * CreatePyramid()
{
    // 3 coordinates in 3D space, 2 texture coordinates
    GLfloat vertices[] = {
        // x     y     z     u     v
        -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.5f, 1.0f
    };

    unsigned int elements[] = {
        0, 1, 4,
        1, 3, 4,
        3, 2, 4,
        2, 0, 4
    };

    Mesh * obj = new Mesh();
    obj->createMesh(vertices,elements,25,12);
    return obj;
}

Mesh * CreateCube()
{
    // 3 coordinates in 3D space, 2 texture coordinates
    GLfloat vertices[] = {
        // x     y     z     u     v
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };

    unsigned int elements[] = {
        0, 1, 2,
        1, 3, 2,
        1, 5, 3,
        5, 7, 3,
        5, 4, 7,
        4, 6, 7,
        4, 0, 6,
        0, 2, 6
    };

    Mesh * obj = new Mesh();
    obj->createMesh(vertices,elements,40,24);
    return obj;
}

Mesh * CreateFloor()
{
    // 3 coordinates in 3D space, 2 texture coordinates
    GLfloat vertices[] = {
        // x     y     z     u     v
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 10.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 10.0f,
        1.0f, 0.0f, 1.0f, 10.0f, 10.0f
    };

    unsigned int elements[] = {
        0, 1, 2,
        1, 3, 2,
    };

    Mesh * obj = new Mesh();
    obj->createMesh(vertices,elements,40,36);
    return obj;
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
    objects.push_back(CreateFloor());
    objects.push_back(CreateCube());
    objects.push_back(CreatePyramid());

    // create texture objects
    Texture grass(std::string(LEARNING_OPENGL_SOURCE_PATH) + "/textures/grass.png");
    grass.loadTexture();
    Texture brick(std::string(LEARNING_OPENGL_SOURCE_PATH) + "/textures/brick.png");
    brick.loadTexture();
    Texture straw(std::string(LEARNING_OPENGL_SOURCE_PATH) + "/textures/straw.png");
    straw.loadTexture();

    // create light objects
    Light light(1.0f, 1.0f, 1.0f, 1.0f); // white light, full intensity

    // create acamera object
    Camera camera(glm::vec3(1.5f,2.5f,3.5f), // centered at the origin
                  glm::vec3(0.0f,1.0f,0.0f), // global up direction
                  -110.0f,                    // initial pitch -> along -Z axis
                  -20.0f,                      // initial yaw -> strictly horizontal
                  10.0f,                     // linear move speed pixel/sec
                  0.05f);                    // rotational move speed a.k.a. mouse sensitivity

    // vertex shader filename
    std::string vShader = std::string(LEARNING_OPENGL_SOURCE_PATH) + "/shaders/vShader.txt";
    // fragment shader filename
    std::string fShader = std::string(LEARNING_OPENGL_SOURCE_PATH) + "/shaders/fShader.txt";
    // create and compile shaders on GPU
    Shader shader;
    shader.createFromFile(vShader, fShader);
    GLuint uniModel, uniView, uniProjection, uniAmbientColor, uniAmbientIntensity;

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
        uniAmbientColor = shader.getUniformAmbientColor();
        uniAmbientIntensity = shader.getUniformAmbientIntensity();

        // copy view and projection matrices to the GPU
        glUniformMatrix4fv(uniView,1,GL_FALSE,glm::value_ptr(camera.viewMatrix()));
        glUniformMatrix4fv(uniProjection,1,GL_FALSE,glm::value_ptr(projection));

        // activate Light
        light.useLight(uniAmbientColor, uniAmbientIntensity);

        // set model matrix for the floor and copy in to the GPU
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(10.0,10.0,10.0));
        model = glm::translate(model, glm::vec3(-0.5f,0.0f,-0.5f));
        glUniformMatrix4fv(uniModel,1,GL_FALSE,glm::value_ptr(model));
        // activate texture
        grass.useTexture();
        // render the floor
        objects[0]->renderMesh();

        // set model matrix for the cube and copy in to the GPU
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(2.0,2.0,2.0));
        model = glm::translate(model, glm::vec3(-0.5f,0.0f,-0.5f));
        glUniformMatrix4fv(uniModel,1,GL_FALSE,glm::value_ptr(model));
        // activate texture
        brick.useTexture();
        // render the cube
        objects[1]->renderMesh();

        // set model matrix for the pyramid and copy in to the GPU
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,2.0f,0.0f));
        glUniformMatrix4fv(uniModel,1,GL_FALSE,glm::value_ptr(model));
        // activate texture
        straw.useTexture();
        // render the cube
        objects[2]->renderMesh();

        // deactivate/unbind shader
        glUseProgram(0);

        // refresh the screen buffer (actual draw)
        mainWindow.swapBuffers();
    }

    for (auto &it : objects)
        delete it;

    return 0;
}
