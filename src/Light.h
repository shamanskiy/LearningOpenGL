#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
private:
    glm::vec3 color;
    GLfloat aIntensity;

public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity);
    ~Light();

    void useLight(GLuint uniAmbientColor, GLuint uniAmbientIntensity);
};
