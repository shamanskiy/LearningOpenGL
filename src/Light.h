#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
private:
    glm::vec3 color;
    glm::vec3 direction;

    GLfloat aIntensity;
    GLfloat dIntensity;

public:
    Light(glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f),
          glm::vec3 lightDirection = glm::vec3(0.0f,1.0f,0.0f),
          GLfloat ambientIntensity = 1.0f, GLfloat diffuseIntensity = 1.0f);
    ~Light();

    void useLight(GLuint uniColor, GLuint uniDirection,
                  GLuint uniAmbientIntensity, GLuint uniDiffuseIntensity);
};
