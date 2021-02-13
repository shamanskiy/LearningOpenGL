#include "Window.h"

#include <iostream>

Window::Window(int windowWidth, int windowHeight) :
    m_width(windowWidth),
    m_height(windowHeight),
    m_name(),
    lastX(0.0f),
    lastY(0.0f),
    changeX(0.0f),
    changeY(0.0f),
    mouseFirstMove(true),
    m_events()
{ }

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Outcome Window::initialize()
{
    // ================================ GLFW ============================//
    if (!glfwInit())
    {
        glfwTerminate();
        return Outcome(false, "GLFW initialization failed");
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile = No Backwards Compatibility (?)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow Forward Compatibility (?)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        glfwTerminate();
        return Outcome(false, "GLFW window creation failed");
    }

    // Set a special pointer stored inside GLFW window which will be used
    // in the callback functions to access EventContainer to store input
    glfwSetWindowUserPointer(m_window,&m_events);
    glfwSetKeyCallback(m_window, handleKeys);
    glfwSetCursorPosCallback(m_window, handleMouse);

    // Bind cursor to the window
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // ================================ GLEW ============================//
    // Make GLEW operate on a given GLFW window
    glfwMakeContextCurrent(m_window);
    // Allow modern extension features (for example?)
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return Outcome(false, "GLEW initialization failed!");
    }

    // ================================ OpenGL ============================//
    // ???
    glEnable(GL_DEPTH_TEST);
    // ???
    glViewport(0, 0, getBufferWidth(), getBufferHeight());

    return Outcome(true);
}

void Window::pollEvents()
{
    glfwPollEvents();
    m_events.setTime(glfwGetTime());
}

int Window::getBufferWidth() const 
{
    int bufferWidth;
    glfwGetFramebufferSize(m_window, &bufferWidth, nullptr);
    return bufferWidth;
}

int Window::getBufferHeight() const {
    int bufferHeight;
    glfwGetFramebufferSize(m_window, nullptr, &bufferHeight);
    return bufferHeight;
}

void Window::handleKeys(GLFWwindow* window, int key, int code,
               int action, int mode)
{
    // Use a pointer stored inside the GLFW window to get access
    // to the EventContainter of the Window class
    EventContainer* events = 
        static_cast<EventContainer *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            events->setKeyState(key, true);
        else if (action == GLFW_RELEASE)
            events->setKeyState(key, false);
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    // a nifty way to get the pointer to the Window class
    // associated with the GLFW window handling the event.
    // Look a bit unsafe though
    /*Window* theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMove)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMove = false;
    }

    theWindow->changeX = xPos - theWindow->lastX;
    theWindow->changeY = theWindow->lastY - yPos;
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;*/
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
