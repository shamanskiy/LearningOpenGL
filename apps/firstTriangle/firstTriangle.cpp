#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "loadShader.h"

int main( void )
{
	if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "My first OpenGL triangle", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create a GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW\n";
        glfwTerminate();
        return -1;
    }
    glewExperimental = GL_TRUE;

    // Vertex data in the memory that we will send to the GPU.
	const GLfloat vertices[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};
    
    // Create a buffer object on the GPU where we will store the raw vertex data.
    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    // Bind our buffer to GL_ARRAY_BUFFER which is a target for vertex data.
    // While the buffer is bound to GL_ARRAY_BUFFER, all data operations targeting GL_ARRAY_BUFFER
    // will operate on our buffer.  
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    // Copy vertex data from the memory to the GPU. It will be stored in out buffer
    // because it is still bound to GL_ARRAY_BUFFER.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create a vertex array object (VAO) on the GPU that will map data from the vertex buffer
    // to the vertex shader.
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
    // Bind the VAO. While it's bound, the following vertex array operations
    // will refer to this VAO.
    glBindVertexArray(vertexArrayID);
    // Activate access to attribute number 1 in the currently bound VAO.
    // By default, all attributes are disabled. If a vertex tries to read
    // from a disabled attribute, it will get a pre-defined const value.
    glEnableVertexAttribArray(1);
    // Copy data from the buffer currently bound to GL_ARRAY_BUFFER to 
    // the currently bound VAO. 
    glVertexAttribPointer(
        1,                       // map to location = 1 in the vertex shader
        3,                       // map 3 consecutive elements to each vertex 
        GL_FLOAT,                // data type
        GL_FALSE,                // don't normalize. Should always be false for float. 
        sizeof(vertices[0]) * 3, // stride between data for each vertex  
        (void *)0                // data offset from the beginning 
        );
    // We are done with the buffer, unbind it. Not necessary but a good practice.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	// Create, compile and activate the shader
	GLuint shaderID = loadShaders("vertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(shaderID);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();    
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangles from the currently bound VAO.
        // Start at vertex 0, draw 3 vertices.
        glDrawArrays(GL_TRIANGLES, 0, 3); 
        
        glfwSwapBuffers(window);
    }

    // free memory on GPU
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteVertexArrays(1, &vertexArrayID);
    
    glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}