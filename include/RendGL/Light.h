#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;

class LightBase
{
public:
    virtual void talkToShader(const Shader & shader) const = 0;
};

class LightDirectional : public LightBase
{
public:
    LightDirectional(glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3 lightDirection = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat ambientIntensity = 1.0f, GLfloat diffuseIntensity = 1.0f);

    virtual void talkToShader(const Shader& shader) const;

private:
    glm::vec3 m_color;
    glm::vec3 m_lightDirection;
    GLfloat m_ambientIntensity;
    GLfloat m_diffuseIntensity;
};