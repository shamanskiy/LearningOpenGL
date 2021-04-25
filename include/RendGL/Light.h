#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

class Shader;

class Light
{
public:
    Light() = default;
    Light(glm::vec3 color, GLfloat intensity);

protected:
    glm::vec3 m_color;
    GLfloat m_intensity;
};

class LightAmbient : public Light
{
public:
    LightAmbient() = default;
    LightAmbient(glm::vec3 color, GLfloat intensity);

    void talkToShader(const Shader& shader) const;
};

class LightDirectional : public Light
{
public:
    LightDirectional() = default;
    LightDirectional(glm::vec3 color, glm::vec3 direction,
        GLfloat intensity);

    void talkToShader(const Shader& shader) const;

private:
    glm::vec3 m_direction;
};

class LightPoint : public Light
{
public:
    LightPoint() = default;
    LightPoint(glm::vec3 color, glm::vec3 position,
        glm::vec3 attenuation, GLfloat intensity);

    void talkToShader(const Shader& shader, int pointLightNumber) const;

private:
    glm::vec3 m_position;
    // attenuation a*D^2 + b*D + c
    glm::vec3 m_attenuation;

    static int m_number;
};

class LightManager
{
public:
    LightManager() = default;

    void setAmbientLight(const LightAmbient& ambient);
    void setDirectionalLight(const LightDirectional& directional);
    void addPointLight(const LightPoint& point);

    void talkToShader(const Shader& shader) const;

private:
    LightAmbient m_ambient;
    LightDirectional m_directional;
    std::vector<LightPoint> m_points;
};