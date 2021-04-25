#include "Light.h"

#include "Shader.h"
#include <algorithm> 

Light::Light(glm::vec3 color, GLfloat intensity) :
    m_color(color),
    m_intensity(intensity)
{}

LightAmbient::LightAmbient(glm::vec3 color, GLfloat intensity) :
    Light(color,intensity)
{}

void LightAmbient::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().ambientLight.color,
        m_color.x, m_color.y, m_color.z);
    glUniform1f(shader.uniforms().ambientLight.intensity, m_intensity);
}

LightDirectional::LightDirectional(glm::vec3 color, glm::vec3 direction,
                                   GLfloat intensity) :
    Light(color, intensity),
    m_direction(glm::normalize(direction))
{}

void LightDirectional::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().diffuseLight.color,
        m_color.x, m_color.y, m_color.z);
    glUniform3f(shader.uniforms().diffuseLight.direction,
        m_direction.x, m_direction.y, m_direction.z);
    glUniform1f(shader.uniforms().diffuseLight.intensity, m_intensity);
}

LightPoint::LightPoint(glm::vec3 color, glm::vec3 position,
    glm::vec3 attenuation, GLfloat intensity) :
    Light(color, intensity),
    m_position(position),
    m_attenuation(attenuation)
{}

void LightPoint::talkToShader(const Shader& shader, int pointLightNumber) const
{
    glUniform3f(shader.uniforms().pointLights[pointLightNumber].color,
        m_color.x, m_color.y, m_color.z);
    glUniform3f(shader.uniforms().pointLights[pointLightNumber].position,
        m_position.x, m_position.y, m_position.z);
    glUniform3f(shader.uniforms().pointLights[pointLightNumber].attenuation,
        m_attenuation.x, m_attenuation.y, m_attenuation.z);
    glUniform1f(shader.uniforms().pointLights[pointLightNumber].intensity, m_intensity);
}

void LightManager::setAmbientLight(const LightAmbient& ambient)
{
    m_ambient = ambient;
}

void LightManager::setDirectionalLight(const LightDirectional& directional)
{
    m_directional = directional;
}

void LightManager::addPointLight(const LightPoint& point)
{
    m_points.push_back(point);
}

void LightManager::talkToShader(const Shader& shader) const
{
    m_ambient.talkToShader(shader);
    m_directional.talkToShader(shader);

    size_t numPointLight = std::min(m_points.size(), MAX_POINT_LIGHTS);
    glUniform1i(shader.uniforms().numPointLights, numPointLight);
    for (int i = 0; i < numPointLight; i++)
        m_points[i].talkToShader(shader, i);
}

