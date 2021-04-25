#include "Light.h"

#include "Shader.h"
#include <algorithm> 

AmbientLight::AmbientLight(glm::vec3 color, GLfloat intensity) :
    m_color(color),
    m_intensity(intensity)
{}

void AmbientLight::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().ambientLight.color,
        m_color.x, m_color.y, m_color.z);
    glUniform1f(shader.uniforms().ambientLight.intensity, m_intensity);
}

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction,
                                   GLfloat intensity) :
    m_color(color),
    m_intensity(intensity),
    m_direction(glm::normalize(direction))
{}

void DirectionalLight::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().diffuseLight.color,
        m_color.x, m_color.y, m_color.z);
    glUniform3f(shader.uniforms().diffuseLight.direction,
        m_direction.x, m_direction.y, m_direction.z);
    glUniform1f(shader.uniforms().diffuseLight.intensity, m_intensity);
}

PointLight::PointLight(glm::vec3 color, glm::vec3 position,
    glm::vec3 attenuation, GLfloat intensity) :
    m_color(color),
    m_intensity(intensity),
    m_position(position),
    m_attenuation(attenuation)
{}

void PointLight::talkToShader(const Shader& shader, int number) const
{
    glUniform3f(shader.uniforms().pointLights[number].color,
        m_color.x, m_color.y, m_color.z);
    glUniform3f(shader.uniforms().pointLights[number].position,
        m_position.x, m_position.y, m_position.z);
    glUniform3f(shader.uniforms().pointLights[number].attenuation,
        m_attenuation.x, m_attenuation.y, m_attenuation.z);
    glUniform1f(shader.uniforms().pointLights[number].intensity, m_intensity);
}

void LightManager::setAmbientLight(const AmbientLight& ambient)
{
    m_ambient = ambient;
}

void LightManager::setDirectionalLight(const DirectionalLight& directional)
{
    m_directional = directional;
}

void LightManager::addPointLight(const PointLight& point)
{
    m_points.push_back(point);
}


void LightManager::talkToShader(const Shader& shader) const
{
    m_ambient.talkToShader(shader);
    m_directional.talkToShader(shader);

    talkAboutPointLights(shader);
}

void LightManager::talkAboutPointLights(const Shader& shader) const
{
    size_t cappedNumberOfLights = std::min(m_points.size(), MAX_POINT_LIGHTS);
    glUniform1i(shader.uniforms().numPointLights, cappedNumberOfLights);
    for (int i = 0; i < cappedNumberOfLights; i++)
        m_points[i].talkToShader(shader, i);
}

