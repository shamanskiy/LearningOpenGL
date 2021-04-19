#include "Light.h"

#include "Shader.h"

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

void LightPoint::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().pointLights[0].color,
        m_color.x, m_color.y, m_color.z);
    glUniform3f(shader.uniforms().pointLights[0].position,
        m_position.x, m_position.y, m_position.z);
    glUniform3f(shader.uniforms().pointLights[0].attenuation,
        m_attenuation.x, m_attenuation.y, m_attenuation.z);
    glUniform1f(shader.uniforms().pointLights[0].intensity, m_intensity);
}
