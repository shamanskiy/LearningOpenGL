#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utils.h"

// Window is a class that creates an actual application window and manages GLFW
class Window
{
private:
    // pointer to the window
    GLFWwindow *mainWindow;
    
    // window size
    GLint width, height;
    // buffer size (actual size of the image)
    GLint bufferWidth, bufferHeight;
    
    // states of all keys on the keyboard in ASCII code
    bool keys[1024];
    
    // mouse state variables: last position and position change
    GLfloat lastX, lastY, changeX, changeY;
    // a flag that this is the first mouse move
    bool mouseFirstMove;
    
public:
    // constructor 
    Window(GLint windowWidth = 600, GLint windowHeight = 800);
    ~Window();
    
    // initialize GLFW context and create the window (constructor does nothing)
    Outcome initialize();
    
    // access functions for the buffer size
    GLint getBufferWidth() { return bufferWidth; }
    GLint getBufferHeight() { return bufferHeight; }
    
    // wrapper function for the closing command
    bool shouldClose() { return glfwWindowShouldClose(mainWindow); }
    // wrapper function for the buffer swap
    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    // process glfw input. should be called at the start of the loop
    void pollEvents() { glfwPollEvents(); }


    
    // access function for the array of key states (returning a pointer - not nice)
    bool * getKeys() { return keys; }
    // access functions for the mouse movement; set movement to zero after reading it (not nice)
    GLfloat getChangeX();
    GLfloat getChangeY();
private:
    // GLFW uses callback functions to handle events and they have to be static
    static void handleKeys(GLFWwindow* window, int key, int code,
                         int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};
