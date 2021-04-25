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
    glm::vec3 m_direction;
    GLfloat m_intensity;
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
    glm::vec3 m_position;
    // attenuation a*D^2 + b*D + c
    glm::vec3 m_attenuation;
    GLfloat m_intensity;
};

class SpotLight
{
public:
    SpotLight() = default;
    SpotLight(glm::vec3 color, glm::vec3 attenuation,
        GLfloat intensity, GLfloat halfAngle,
        bool isOn);

    void talkToShader(const Shader & shader) const;

private:
    glm::vec3 m_color;
    // attenuation a*D^2 + b*D + c
    glm::vec3 m_attenuation;
    GLfloat m_intensity;
    GLfloat m_halfAngle;
    bool m_isOn;
};


class LightManager
{
public:
    void setAmbientLight(const AmbientLight& ambientLight);
    void setDirectionalLight(const DirectionalLight& directionalLight);
    void addPointLight(const PointLight& pointLight);
    void setSpotLight(const SpotLight& spotLight);

    void talkToShader(const Shader& shader) const;

private:
    void talkAboutPointLights(const Shader& shader) const;
private:
    AmbientLight m_ambientLight;
    DirectionalLight m_directionalLight;
    std::vector<PointLight> m_pointLights;
    SpotLight m_spotLight;
};