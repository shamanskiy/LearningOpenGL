#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

class Shader;


class AmbientLight
{
public:
    AmbientLight() = default;
    AmbientLight(glm::vec3 color, GLfloat intensity);

    void talkToShader(const Shader& shader) const;

private:
    glm::vec3 m_color;
    GLfloat m_intensity;
};

class DirectionalLight
{
public:
    DirectionalLight() = default;
    DirectionalLight(glm::vec3 color, glm::vec3 direction,
        GLfloat intensity);

    void talkToShader(const Shader& shader) const;

private:
    glm::vec3 m_color;
    GLfloat m_intensity;
    glm::vec3 m_direction;
};

class PointLight
{
public:
    PointLight() = default;
    PointLight(glm::vec3 color, glm::vec3 position,
        glm::vec3 attenuation, GLfloat intensity);

    void talkToShader(const Shader& shader, int number) const;

private:
    glm::vec3 m_color;
    GLfloat m_intensity;
    glm::vec3 m_position;
    // attenuation a*D^2 + b*D + c
    glm::vec3 m_attenuation;
};

class LightManager
{
public:
    void setAmbientLight(const AmbientLight& ambient);
    void setDirectionalLight(const DirectionalLight& directional);
    void addPointLight(const PointLight& point);

    void talkToShader(const Shader& shader) const;

private:
    void talkAboutPointLights(const Shader& shader);
private:
    AmbientLight m_ambient;
    DirectionalLight m_directional;
    std::vector<PointLight> m_points;
};