#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() 
{
    // Try to initialize GLFW. GLFW (Graphics Library Framework) is a utility library
    // that communicates with the operation system and provides the context for OpenGL to run it.
    // Essentially, it creates a window and processes mouse and keyboard input.
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    // Define what version and components of OpenGL we want to use.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glewExperimental = GL_TRUE;

    // Create a window.
    GLFWwindow* window = glfwCreateWindow(800, 600, "My first OpenGL window", nullptr, nullptr);
    if (!window) {

        std::cout << "Failed to create a GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Try to initialize GLEW. GLEW (OpenGL Extension Wrangler Library) adapts OpenGL to your
    // OS and hardware. 
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    // An infinite loop that runs until you click on the close button of the window. 
    // Here is where the magic happens.
    while(!glfwWindowShouldClose(window)) 
    {
        // Check the input.
        glfwPollEvents();

        // Clear the back/hidden buffer before drawing to it.
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap the front/displayed buffer with the back buffer to display the latter.
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}