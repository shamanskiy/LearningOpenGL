#pragma once

#include <utility>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EventContainer;
class Shader;

// Camera receives user input from mouse and keyboard and
// lets one navigate the scene
class Camera
{
public:
    Camera(glm::vec3 initialPosition = glm::vec3(0.0f,0.0f,0.0f),
           GLfloat initialYaw = 0.0f, GLfloat initialPitch = 0.0f,
           GLfloat moveSpeed = 1.0f, GLfloat turnSpeed = 1.0f,
           glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f));

    void processEvents(const EventContainer& events);
    void talkToShader(const Shader& shader) const;

    glm::vec3 position() const { return m_state.pos; }
    glm::vec3 front() const { return m_front; }
    glm::vec3 up() const { return m_up; }
    glm::vec3 right() const { return m_right; }

private:
    void processKeys(const EventContainer& events);
    void processMouse(const EventContainer& events);

    void updatePosition(const glm::vec3& direction, GLfloat timeStep);
    // compute new orientation from yaw and pitch
    void updateOrientation();

    // compute view matrix from the current position and orientation
    glm::mat4 viewMatrix() const;

private:
    struct CameraState
    {
        glm::vec3 pos;
        GLfloat yaw;
        GLfloat pitch;
    };

private:
    CameraState m_state;
    // this state is used for camera reset
    CameraState m_initialState;
    
    // current orientation
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;

    // global Y-direction
    glm::vec3 m_worldUp;

    // movement speed used for keys
    GLfloat m_moveSpeed;
    // turning speed used for mouse
    GLfloat m_turnSpeed;
};
