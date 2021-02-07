#include "Window.h"

#include <iostream>

Window::Window(GLint windowWidth, GLint windowHeight) :
    width(windowWidth),
    height(windowHeight),
    lastX(0.0f),
    lastY(0.0f),
    changeX(0.0f),
    changeY(0.0f),
    mouseFirstMove(true),
    m_input()
{
    for (size_t i = 0; i < 1024; ++i)
        keys[i] = false;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

Outcome Window::initialize()
{
    // ================================ GLFW ============================//
    // Initialise GLFW
    if (!glfwInit())
    {
        glfwTerminate();
        return Outcome(false, "GLFW initialization failed");
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
    mainWindow = glfwCreateWindow(width, height, "OpenGL App", nullptr, nullptr);
    if (!mainWindow)
    {
        glfwTerminate();
        return Outcome(false, "GLFW window creation failed");
    }

    // Get Buffer Size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set auxiliary pointer for the static function "handleKeys" to access the Window class instance
    glfwSetWindowUserPointer(mainWindow,this);

    // set a callback function to handle keyboard events
    glfwSetKeyCallback(mainWindow, handleKeys);

    // set a callback function to handle mouse events
    glfwSetCursorPosCallback(mainWindow, handleMouse);
    // bind cursor to the window
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // ================================ GLEW ============================//

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return Outcome(false, "GLEW initialization failed!");
    }

    // ================================ OpenGL ============================//

    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    return Outcome(true);
}

void Window::handleKeys(GLFWwindow* window, int key, int code,
               int action, int mode)
{
    // a nifty way to get the pointer to the Window class
    // associated with the GLFW window handling the event.
    // Looks a bit unsafe though
    Window* theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            theWindow->keys[key] = true;
        else if (action == GLFW_RELEASE)
            theWindow->keys[key] = false;
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    // a nifty way to get the pointer to the Window class
    // associated with the GLFW window handling the event.
    // Look a bit unsafe though
    Window* theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMove)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMove = false;
    }

    theWindow->changeX = xPos - theWindow->lastX;
    theWindow->changeY = theWindow->lastY - yPos;
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}


GLfloat Window::getChangeX()
{
    GLfloat xChange = changeX;
    changeX = 0.0f;
    return xChange;
}

GLfloat Window::getChangeY()
{
    GLfloat yChange = changeY;
    changeY = 0.0f;
    return yChange;
}

void Window::pollEvents()
{
    glfwPollEvents();
    m_input.setTime(glfwGetTime());
}
