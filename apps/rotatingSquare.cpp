// This is a demo app that shows basic OpenGL steps to draw a single triangle.
#include "Shader.h"
#include "Window.h"
#include "Config.h"
#include "Mesh.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    Window window(600, 600, "Rotating Square");

    const ShaderProgram shader(SHADERS_DIR + "rotatingSquareVertex.glsl", SHADERS_DIR + "rotatingSquareFragment.glsl");
    shader.activateShader();

    const std::vector<GLfloat> vertices {
        -0.7f, -0.7f, 0.0f, // vertex 1
         0.7f, -0.7f, 0.0f, // vertex 2
        -0.7f,  0.7f, 0.0f, // vertex 3
         0.7f,  0.7f, 0.0f, // vertex 4
    };
    const std::vector<GLuint> indices {
        0, 1, 2, // triangle 1
        1, 2, 3  // triangle 2
    };
    const Mesh square(vertices, indices, VertexData::POSITION);

    // Get location index of the "rotation" variable in the shader
    GLint rotationLocation = glGetUniformLocation(shader.id(), "rotation");
    glm::mat4 rotation = glm::mat4(1.0f);
        
    while (!window.shouldClose()) {
        window.pollEvents();

        // increment the rotation at every iteration
        rotation = glm::rotate(rotation, glm::radians(0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
        // send the rotation matrix to the GPU using the known location
        glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, glm::value_ptr(rotation));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        square.render();   
        
        window.swapBuffers();
    }
}