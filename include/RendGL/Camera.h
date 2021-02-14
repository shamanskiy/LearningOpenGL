#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EventContainer;

// Camera is a class for movement around and interaction with a rendered scene
class Camera
{
private:
    // camera position in 3D space
    glm::vec3 position;
    // original position for camera reset
    glm::vec3 resetPosition;
    // camera Z-vector in the global coordinates
    glm::vec3 front;
    // camera Y-vector in the global coordinates
    glm::vec3 up;
    // camera X-vector in the global coordinates
    glm::vec3 right;
    // global Y-direction
    const glm::vec3 worldUp;

    // current yaw angle (left-right)
    GLfloat yaw;
    // original yaw for camera reset
    GLfloat resetYaw;
    // current pitch angle (bottom-top)
    GLfloat pitch;
    // original pitch for camera reset
    GLfloat resetPitch;

    // moving speed (keyboard)
    const GLfloat moveSpeed;
    // turning speed (mouse)
    const GLfloat turnSpeed;

public:
    // constructor
    Camera(glm::vec3 initialPosition = glm::vec3(0.0f,0.0f,0.0f),
           glm::vec3 worldUp = glm::vec3(0.0f,1.0f,0.0f),
           GLfloat initialYaw = -90.0f, GLfloat initialPitch = 0.0f,
           GLfloat moveSpeed_ = 10.0f, GLfloat turnSpeed_ = 0.05f);
    // destructor
    ~Camera();

    // process key controls (move camera)
    void keyControl(const EventContainer & events);

    // process mouse controls (rotate camera)
    void mouseControl(GLfloat xChange, GLfloat yChange);

    // return a view matrix corresponding to the current camera's position and orientation
    glm::mat4 viewMatrix() { return glm::lookAt(position, position+front, up); }
    
    const glm::vec3 & cameraPosition() const { return position; }

private:

    // update camera position and orientation provided new yaw and pitch
    void update();

};
