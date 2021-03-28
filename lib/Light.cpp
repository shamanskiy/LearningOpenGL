#include "Light.h"

#include "Shader.h"

Light::Light(glm::vec3 lightColor, glm::vec3 lightDirection,
             GLfloat ambientIntensity, GLfloat diffuseIntensity) :
    color(lightColor),
    direction(glm::normalize(lightDirection)),
    aIntensity(ambientIntensity),
    dIntensity(diffuseIntensity)
{}

void Light::talkToShader(const Shader& shader) const
{
    glUniform3f(shader.uniforms().lightColor,
        color.x, color.y, color.z);
    glUniform3f(shader.uniforms().lightDirection,
        direction.x, direction.y, direction.z);
    glUniform1f(shader.uniforms().ambientIntensity, aIntensity);
    glUniform1f(shader.uniforms().diffuseIntensity, dIntensity);
}
