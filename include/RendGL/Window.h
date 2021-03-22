#pragma once

#include "Utils.h"

// Window is a class that creates an actual application window and manages GLFW
class Window
{
public:
    Window(int windowWidth = 600, int windowHeight = 800,
        const std::string& windowName = "MyApp");
    ~Window();


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

private:
    // callback functions for GLFW
    static void keyboardCallback(GLFWwindow* window, int key, int code,
        int action, int mode);
    static void cursorCallback(GLFWwindow* window, double xPos, double yPos);
    static void bufferResizeCallback(GLFWwindow* window, int newWidth, int newHeight);

    // Initialize GLFW and GLEW, and create the window
    void initialize();

private:
    // GLFW window, its size and name
    GLFWwindow *m_window;
    int m_width, m_height;
    std::string m_name;
    
    // GLFW events processed during this frame,
    // e.g. key pressed, mouse moved, time elapsed.
    EventContainer m_events;
};
