#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Camera is a class for moving and interaction with a rendered scene
class Camera
{
private:
    // camera position in 3D space
    glm::vec3 position;
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
    // current pitch angle (bottom-top)
    GLfloat pitch;
    
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
    void keyControl(bool * keys, GLfloat delta_t);
    
    // process mouse controls (rotate camera)
    void mouseControl(GLfloat xChange, GLfloat yChange);
    
    // return a view matrix corresponding to the current camera's position and orientation
    glm::mat4 viewMatrix() { return glm::lookAt(position, position+front, up); }
    
private:
    
    // update camera position and orientation provided new yaw and pitch
    void update();
    
};