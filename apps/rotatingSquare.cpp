// This is a demo app that shows basic OpenGL steps to draw a single triangle.
#include "Shader.h"
#include "Window.h"
#include "Config.h"
#include "Model.h"

#include <vector>

int main()
{
    Window window(600, 600, "Rotating Square");

    // Create, compile and activate the shader
    const ShaderProgram shader(SHADERS_DIR + "rotatingSquareVertex.glsl", SHADERS_DIR + "rotatingSquareFragment.glsl");
    shader.activateShader();

    const std::vector<GLfloat> vertices{
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // vertex 1
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // vertex 2
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // vertex 3
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f  // vertex 4
    };
    const std::vector<GLuint> indices{0,1,2, 1,2,3};

    const Mesh triangle(vertices, indices);
    
    while (!window.shouldClose())
    {
        window.pollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        triangle.render();   
        
        window.swapBuffers();
    }
}