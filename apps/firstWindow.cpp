// This is a demo app that shows how to create a GLFW window.
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() 
{
    // GLFW (Graphics Library Framework) is a utility library that creates 
    // an OS window for OpenGL to draw to and processes mouse and keyboard input.
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    // Set OpenGL version and compatability modes.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glewExperimental = GL_TRUE;

    // Create a GLFW window and tell OpenGL to draw to it.
    GLFWwindow* window = glfwCreateWindow(600, 400, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create a GLFW window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLEW (OpenGL Extension Wrangler) checks and manages
    // which OpenGL features are available on your OS and GPU.  
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    // An infinite loop that runs until you click on the close button of the window. 
    while(!glfwWindowShouldClose(window)) 
    {
        // Check the mouse and keyboard input.
        glfwPollEvents();
        // Clear the back buffer before rendering to it.
        glClear(GL_COLOR_BUFFER_BIT);

        // render something here...

        // Swap the front buffer with the back buffer to display the latter.
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}