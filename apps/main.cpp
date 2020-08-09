#include <iostream>
#include <chrono>
#include <thread>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow Forward Compatbility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    // Get Buffer Size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Loop until window closed
    int i = 0;
    auto startLoop = chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(mainWindow))
    {
        auto poll = chrono::high_resolution_clock::now();
        cout << "frame " << i << endl;
        // Get + Handle user input events
        cout << "poll\n";
        glfwPollEvents();

        auto clear = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedPoll = clear - poll;
        cout << "Elapsed time: " << elapsedPoll.count()*1000 << " ms\n";

        // Clear window
        cout << "clear\n";
        glClearColor(1.0f/255*(rand()%256) ,1.0f/255*((rand()+100)%256), 1.0f/255*((rand()-100)%256), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        auto swap = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedClear = swap - clear;
        cout << "Elapsed time: " << elapsedClear.count()*1000 << " ms\n";
        
        //this_thread::sleep_for(chrono::milliseconds(100));
        cout << "swap\n";
        
        auto sleep = chrono::high_resolution_clock::now();
        glfwSwapBuffers(mainWindow);
        i++;
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedEnd = end - sleep;
        cout << "Elapsed time: " << elapsedEnd.count()*1000 << " ms\n";

        
    }
    auto endLoop = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedLoop = endLoop - startLoop;
    cout << "Elapsed time: " << elapsedLoop.count() << " s\n";
    cout << "FPS: " << i/elapsedLoop.count() << endl;
    
    

    return 0;
}
