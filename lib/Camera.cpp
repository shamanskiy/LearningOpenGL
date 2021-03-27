#include "Camera.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include <Utils.h>


Camera::Camera(glm::vec3 initialPosition,
              GLfloat initialYaw, GLfloat initialPitch,
              GLfloat moveSpeed_, GLfloat turnSpeed_,
              glm::vec3 worldUp_) :
    position(initialPosition),
    resetPosition(initialPosition),
    worldUp(worldUp_),
    yaw(initialYaw),
    resetYaw(initialYaw),
    pitch(initialPitch),
    resetPitch(initialPitch),
    moveSpeed(moveSpeed_),
    turnSpeed(turnSpeed_)
{
    update();
}

void Camera::keyControl(const EventContainer& events)
{
    if (events.keyState(GLFW_KEY_W))
        position += front * moveSpeed * events.timeStep();
    if (events.keyState(GLFW_KEY_S))
        position -= front * moveSpeed * events.timeStep();
    if (events.keyState(GLFW_KEY_D))
        position += right * moveSpeed * events.timeStep();
    if (events.keyState(GLFW_KEY_A))
        position -= right * moveSpeed * events.timeStep();
    if (events.keyState(GLFW_KEY_UP))
        position += up * moveSpeed * events.timeStep();
    if (events.keyState(GLFW_KEY_DOWN))
        position -= up * moveSpeed * events.timeStep();
    
    if (events.keyState(GLFW_KEY_R))
    {
        position = resetPosition;
        yaw = resetYaw;
        pitch = resetPitch;
        update();
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    yaw += xChange * turnSpeed;
    
    pitch += yChange * turnSpeed;
    // limit pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    update();
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front = glm::normalize(front);
    
    right = glm::normalize(glm::cross(front,worldUp));
    up = glm::normalize(glm::cross(right,front));
}
