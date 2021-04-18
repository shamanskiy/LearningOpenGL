#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;

class Light
{
public:
    Light(glm::vec3 color, GLfloat intensity);
    virtual void talkToShader(const Shader & shader) const = 0;

protected:
    glm::vec3 m_color;
    GLfloat m_intensity;
};

class LightAmbient : public Light
{
public:
    LightAmbient(glm::vec3 color, GLfloat intensity);

    void talkToShader(const Shader& shader) const override;
};

class LightDirectional : public Light
{
public:
    LightDirectional(glm::vec3 color, glm::vec3 direction,
        GLfloat intensity);

    void talkToShader(const Shader& shader) const override;

private:
    glm::vec3 m_direction;
};

class LightPoint : public Light
{
public:
    LightPoint(glm::vec3 color, glm::vec3 position,
        glm::vec3 attenuation, GLfloat intensity);

    void talkToShader(const Shader& shader) const override;

private:
    glm::vec3 m_position;
    // attenuation a*D^2 + b*D + c
    glm::vec3 m_attenuation;
};