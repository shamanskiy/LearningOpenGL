#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utils.h"

// Window is a class that creates an actual application window and manages GLFW
class Window
{
public:
    Window(int windowWidth = 600, int windowHeight = 800);
    ~Window();

    // Only has effect if called before initialize()
    void setWindowName(const std::string& windowName) { m_name = windowName; }
    void setWindowName(std::string&& windowName) { m_name = std::move(windowName); }

    // Initialize GLFW and GLEW, and create the window (constructor does nothing)
    Outcome initialize();

    // Process GLFW input. Should be called at the start of the loop
    void pollEvents();

    // Give access to all GLFW events processed during this frame,
    // e.g. key pressed, mouse moved, time elapsed.
    const EventContainer& events() { return m_events; }

    // Swap OpenGL buffers to update the window image
    void swapBuffers() { glfwSwapBuffers(m_window); }

    // Check if the should close (e.g. "close" button was clicked)
    bool shouldClose() { return glfwWindowShouldClose(m_window); }

    // Access functions for the buffer size.
    // Buffer size may differ from the window size,
    // e.g. bSize = wSize - borders
    int getBufferWidth() const;
    int getBufferHeight() const;

    GLfloat getChangeX();
    GLfloat getChangeY();

private:
    // actual window
    GLFWwindow *m_window;
    
    // window size
    int m_width, m_height;

    // window name (displayed on the top border)
    std::string m_name;
    
    // GLFW events processed during this frame,
    // e.g. key pressed, mouse moved, time elapsed.
    EventContainer m_events;


    // mouse state variables: last position and position change
    GLfloat lastX, lastY, changeX, changeY;
    // a flag that this is the first mouse move
    bool mouseFirstMove;

private:
    // callback functions for GLFW keyboard and mouse events
    static void handleKeys(GLFWwindow* window, int key, int code,
                         int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};
