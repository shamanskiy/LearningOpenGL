#include "Light.h"

#include "Shader.h"
#include "Utils.h"
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
    m_direction(glm::normalize(direction)),
    m_intensity(intensity)
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
    m_position(position),
    m_attenuation(attenuation),
    m_intensity(intensity)
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

SpotLight::SpotLight(glm::vec3 color, glm::vec3 attenuation,
    GLfloat intensity, GLfloat halfAngle,
    bool isOn) :
    m_color(color),
    m_attenuation(attenuation),
    m_intensity(intensity),
    m_halfAngleCos(cos(glm::radians(halfAngle))),
    m_isOn(isOn)
{}

void SpotLight::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().spotLight.color,
        m_color.x, m_color.y, m_color.z);
    glUniform3f(shader.uniforms().spotLight.attenuation,
        m_attenuation.x, m_attenuation.y, m_attenuation.z);
    glUniform1f(shader.uniforms().spotLight.intensity, m_intensity);
    glUniform1f(shader.uniforms().spotLight.halfAngleCos, m_halfAngleCos);
    glUniform1f(shader.uniforms().spotLight.isOn, m_isOn);
}

void SpotLight::toggleOnOff()
{
    m_isOn = !m_isOn;
}



void LightManager::setAmbientLight(const AmbientLight& ambientLight)
{
    m_ambientLight = ambientLight;
}

void LightManager::setDirectionalLight(const DirectionalLight& directionalLight)
{
    m_directionalLight = directionalLight;
}

void LightManager::addPointLight(const PointLight& pointLight)
{
    m_pointLights.push_back(pointLight);
}

void LightManager::setSpotLight(const SpotLight& spotLight)
{
    m_spotLight = spotLight;
}


void LightManager::talkToShader(const Shader& shader) const
{
    m_ambientLight.talkToShader(shader);
    m_directionalLight.talkToShader(shader);

    talkAboutPointLights(shader);

    m_spotLight.talkToShader(shader);
}

void LightManager::talkAboutPointLights(const Shader& shader) const
{
    size_t cappedNumberOfLights = std::min(m_pointLights.size(), MAX_POINT_LIGHTS);
    glUniform1i(shader.uniforms().numPointLights, cappedNumberOfLights);
    for (int i = 0; i < cappedNumberOfLights; i++)
        m_pointLights[i].talkToShader(shader, i);
}

void LightManager::processEvents(const EventContainer& events)
{
    if (events.keyState(GLFW_KEY_F) && m_spotLightSwitch)
        m_spotLight.toggleOnOff();

    if (events.keyState(GLFW_KEY_F))
        m_spotLightSwitch = false;

    if (!events.keyState(GLFW_KEY_F) && !m_spotLightSwitch)
        m_spotLightSwitch = true;

}