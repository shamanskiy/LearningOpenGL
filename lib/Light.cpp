#include "Light.h"

#include "Utils.h"
#include <algorithm> 

AmbientLight::AmbientLight(glm::vec3 color, GLfloat intensity) :
    m_color(color),
    m_intensity(intensity)
{}

void AmbientLight::talkToShader(GLuint shader) const
{
    glUniform3f(glGetUniformLocation(shader, "ambientLight.color"),
        m_color.x, m_color.y, m_color.z);
    glUniform1f(glGetUniformLocation(shader, "ambientLight.intensity"), m_intensity);
}

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction,
                                   GLfloat intensity) :
    m_color(color),
    m_direction(glm::normalize(direction)),
    m_intensity(intensity)
{}

void DirectionalLight::talkToShader(GLuint shader) const
{
    glUniform3f(glGetUniformLocation(shader, "diffuseLight.color"),
        m_color.x, m_color.y, m_color.z);
    glUniform3f(glGetUniformLocation(shader, "diffuseLight.direction"),
        m_direction.x, m_direction.y, m_direction.z);
    glUniform1f(glGetUniformLocation(shader, "diffuseLight.intensity"), m_intensity);
}

PointLight::PointLight(glm::vec3 color, glm::vec3 position,
    glm::vec3 attenuation, GLfloat intensity) :
    m_color(color),
    m_position(position),
    m_attenuation(attenuation),
    m_intensity(intensity)
{}

void PointLight::talkToShader(GLuint shader, int number) const
{
    char locBuff[100] = { '\0' };

    snprintf(locBuff, sizeof(locBuff), "pointLights[%d].color", number);    
    glUniform3f(glGetUniformLocation(shader, locBuff),
        m_color.x, m_color.y, m_color.z);

    snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", number);
    glUniform3f(glGetUniformLocation(shader, locBuff),
        m_position.x, m_position.y, m_position.z);

    snprintf(locBuff, sizeof(locBuff), "pointLights[%d].attenuation", number);    
    glUniform3f(glGetUniformLocation(shader, locBuff),
        m_attenuation.x, m_attenuation.y, m_attenuation.z);

    snprintf(locBuff, sizeof(locBuff), "pointLights[%d].intensity", number); 
    glUniform1f(glGetUniformLocation(shader, locBuff), m_intensity);
}

SpotLight::SpotLight(glm::vec3 color, glm::vec3 attenuation,
    GLfloat intensity, GLfloat halfAngle, GLfloat verticalOffset,
    bool isOn) :
    m_color(color),
    m_attenuation(attenuation),
    m_intensity(intensity),
    m_halfAngleCos(cos(glm::radians(halfAngle))),
    m_verticalOffset(verticalOffset),
    m_isOn(isOn)
{}

void SpotLight::talkToShader(GLuint shader) const
{
    glUniform3f(glGetUniformLocation(shader, "spotLight.color"),
        m_color.x, m_color.y, m_color.z);
    glUniform3f(glGetUniformLocation(shader, "spotLight.attenuation"),
        m_attenuation.x, m_attenuation.y, m_attenuation.z);
    glUniform1f(glGetUniformLocation(shader, "spotLight.intensity"), m_intensity);
    glUniform1f(glGetUniformLocation(shader, "spotLight.halfAngleCos"), m_halfAngleCos);
    glUniform1f(glGetUniformLocation(shader, "spotLight.verticalOffset"), m_verticalOffset);
    glUniform1i(glGetUniformLocation(shader, "spotLight.isOn"), m_isOn.state());
}

void SpotLight::switchOnOff(bool signal)
{
    if (signal)
        m_isOn.push();
    else
        m_isOn.release();
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


void LightManager::talkToShader(GLuint shader) const
{
    m_ambientLight.talkToShader(shader);
    m_directionalLight.talkToShader(shader);

    talkAboutPointLights(shader);

    m_spotLight.talkToShader(shader);
}

void LightManager::talkAboutPointLights(GLuint shader) const
{
    size_t cappedNumberOfLights = std::min(m_pointLights.size(), MAX_POINT_LIGHTS);
    glUniform1i(glGetUniformLocation(shader, "numPointLights"), cappedNumberOfLights);
    for (int i = 0; i < cappedNumberOfLights; i++)
        m_pointLights[i].talkToShader(shader, i);
}

void LightManager::processEvents(const EventContainer& events)
{
    if (events.keyState(GLFW_KEY_F))
        m_spotLight.switchOnOff(true);
    else
        m_spotLight.switchOnOff(false);

}