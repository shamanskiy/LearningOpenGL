#include "Camera.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include <Utils.h>


Camera::Camera(glm::vec3 initialPosition,
              GLfloat initialYaw, GLfloat initialPitch,
              GLfloat moveSpeed, GLfloat turnSpeed,
              glm::vec3 worldUp) :
    m_state{ initialPosition, initialYaw, initialPitch },
    m_initialState{ m_state },
    m_worldUp(worldUp),
    m_moveSpeed(moveSpeed),
    m_turnSpeed(turnSpeed)
{
    updateOrientation();
}

void Camera::processEvents(const EventContainer& events)
{
    processKeys(events);
    processMouse(events);
}

glm::mat4 Camera::viewMatrix() const 
{ 
    return glm::lookAt(m_state.pos, m_state.pos + m_front, m_up); 
}


void Camera::updatePosition(const glm::vec3& direction, GLfloat timeStep)
{
    m_state.pos += direction * timeStep * m_moveSpeed;
}

void Camera::processKeys(const EventContainer& events)
{
    auto dt = events.timeStep();
    if (events.keyState(GLFW_KEY_W))
        updatePosition(m_front, dt);
    if (events.keyState(GLFW_KEY_S))
        updatePosition(m_front, -dt);
    if (events.keyState(GLFW_KEY_D))
        updatePosition(m_right, dt);
    if (events.keyState(GLFW_KEY_A))
        updatePosition(m_right, -dt);
    if (events.keyState(GLFW_KEY_UP))
        updatePosition(m_up, dt);
    if (events.keyState(GLFW_KEY_DOWN))
        updatePosition(m_up, -dt);
    
    if (events.keyState(GLFW_KEY_R))
    {
        m_state = m_initialState;
        updateOrientation();
    }
}

void Camera::processMouse(const EventContainer& events)
{
    m_state.yaw += events.cursorPositionChangeX() * m_turnSpeed;
    m_state.pitch += events.cursorPositionChangeY() * m_turnSpeed;
    // limit pitch
    if (m_state.pitch > 89.0f)
        m_state.pitch = 89.0f;
    if (m_state.pitch < -89.0f)
        m_state.pitch = -89.0f;
    
    updateOrientation();
}

void Camera::updateOrientation()
{
    m_front.x = cos(glm::radians(m_state.yaw)) * cos(glm::radians(m_state.pitch));
    m_front.z = sin(glm::radians(m_state.yaw)) * cos(glm::radians(m_state.pitch));
    m_front.y = sin(glm::radians(m_state.pitch));
    m_front = glm::normalize(m_front);
    
    m_right = glm::normalize(glm::cross(m_front,m_worldUp));
    m_up = glm::normalize(glm::cross(m_right,m_front));
}
