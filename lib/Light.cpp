#include "Light.h"

#include "Shader.h"

LightDirectional::LightDirectional(glm::vec3 lightColor, glm::vec3 lightDirection,
                                   GLfloat ambientIntensity, GLfloat diffuseIntensity) :
    m_color(lightColor),
    m_lightDirection(glm::normalize(lightDirection)),
    m_ambientIntensity(ambientIntensity),
    m_diffuseIntensity(diffuseIntensity)
{}

void LightDirectional::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().lightColor,
        m_color.x, m_color.y, m_color.z);
    glUniform3f(shader.uniforms().lightDirection,
        m_lightDirection.x, m_lightDirection.y, m_lightDirection.z);
    glUniform1f(shader.uniforms().ambientIntensity, m_ambientIntensity);
    glUniform1f(shader.uniforms().diffuseIntensity, m_diffuseIntensity);
}
