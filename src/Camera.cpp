#include "Camera.h"

#include <GLFW/glfw3.h>
#include <iostream>


Camera::Camera(glm::vec3 initialPosition, glm::vec3 worldUp_,
              GLfloat initialYaw, GLfloat initialPitch,
              GLfloat moveSpeed_, GLfloat turnSpeed_) :
    position(initialPosition),
    worldUp(worldUp_),
    yaw(initialYaw),
    pitch(initialPitch),
    moveSpeed(moveSpeed_),
    turnSpeed(turnSpeed_)
{
    update();
}

Camera::~Camera()
{
    
}

void Camera::keyControl(bool *keys, GLfloat delta_t)
{
    if (keys[GLFW_KEY_W])
        position += front * moveSpeed * delta_t;
    if (keys[GLFW_KEY_S])
        position -= front * moveSpeed * delta_t;
    if (keys[GLFW_KEY_D])
        position += right * moveSpeed * delta_t;
    if (keys[GLFW_KEY_A])
        position -= right * moveSpeed * delta_t;
    if (keys[GLFW_KEY_UP])
        position += up * moveSpeed * delta_t;
    if (keys[GLFW_KEY_DOWN])
        position -= up * moveSpeed * delta_t;
    
    if (keys[GLFW_KEY_R])
    {
        position = glm::vec3(0.0f,0.0f,0.0f);
        yaw = -90;
        pitch = 0;
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
