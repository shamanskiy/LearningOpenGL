// This is a demo app that shows basic OpenGL steps to draw a single triangle.
#include "Shader.h"
#include "Window.h"
#include "Config.h"

int main()
{
    Window window(600, 400, "First Triangle");

    // Create, compile and activate the shader
    ShaderProgram shader(SHADERS_DIR + "firstTriangleVertex.glsl", SHADERS_DIR + "firstTriangleFragment.glsl");
    shader.activateShader();

    // Vertex data in the memory that we will send to the GPU.
    const GLfloat vertices[] = { 
        -1.0f, -1.0f, 0.0f, // vertex 1
         1.0f, -1.0f, 0.0f, // vertex 2
         0.0f,  1.0f, 0.0f, // vertex 3
    };
    
    // Create a buffer object on the GPU where we will store the raw vertex data.
    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    // Bind the buffer to GL_ARRAY_BUFFER, which is a target for vertex data.
    // While the buffer is bound to GL_ARRAY_BUFFER, all data operations targeting 
    // GL_ARRAY_BUFFER will operate on this buffer.   
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    // Copy vertex data from the memory to the GPU. The data will be stored 
    // in the buffer that is bound to GL_ARRAY_BUFFER.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create a vertex array object (VAO) on the GPU that will map data from 
    // the vertex buffer to the vertex shader.
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    // Bind the VAO. While it's bound, the following vertex array operations
    // will refer to this VAO.
    glBindVertexArray(vertexArrayID);
    // Activate shader access to attribute 1 in the bound VAO. By default,
    // all attributes are disabled. Attribute 1 corresponds to (location = 1)
    // in the vertex shader.
    glEnableVertexAttribArray(1);
    // Describe the vertex layout in the buffer and copy the vertices to VAO.
    glVertexAttribPointer(
        1,                       // map to (location = 1) in the vertex shader
        3,                       // map 3 consecutive elements to a single vertex 
        GL_FLOAT,                // data type
        GL_FALSE,                // no need to normalize, always false for float data 
        sizeof(vertices[0]) * 3, // stride between data for each vertex  
        (void *)0                // data offset from the beginning 
        );
    // We are done with the buffer, unbind it. Not necessary but a good practice.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    while (!window.shouldClose())
    {
        window.pollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw triangles from the currently bound VAO.
        // Start at vertex 0, draw 3 vertices.
        glDrawArrays(GL_TRIANGLES, 0, 3); 
        
        window.swapBuffers();
    }

    // free memory on the GPU
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteVertexArrays(1, &vertexArrayID);
}