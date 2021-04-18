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
    glUniform3f(shader.uniforms().lightAmbientColor,
        m_color.x, m_color.y, m_color.z);
    glUniform1f(shader.uniforms().lightAmbientIntensity, m_intensity);
}

LightDirectional::LightDirectional(glm::vec3 color, glm::vec3 direction,
                                   GLfloat intensity) :
    Light(color, intensity),
    m_direction(glm::normalize(direction))
{}

void LightDirectional::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().lightDiffuseColor,
        m_color.x, m_color.y, m_color.z);
    glUniform3f(shader.uniforms().lightDiffuseDirection,
        m_direction.x, m_direction.y, m_direction.z);
    glUniform1f(shader.uniforms().lightDiffuseIntensity, m_intensity);
}

LightPoint::LightPoint(glm::vec3 color, glm::vec3 position,
    GLfloat intensity) :
    Light(color, intensity),
    m_position(position)
{}

void LightPoint::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().lightPointColor,
        m_color.x, m_color.y, m_color.z);
    glUniform3f(shader.uniforms().lightPointPosition,
        m_position.x, m_position.y, m_position.z);
    glUniform1f(shader.uniforms().lightPointIntensity, m_intensity);
}
