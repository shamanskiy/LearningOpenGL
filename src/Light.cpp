#include "Light.h"

Light::Light(glm::vec3 lightColor, glm::vec3 lightDirection,
             GLfloat ambientIntensity, GLfloat diffuseIntensity) :
    color(lightColor),
    direction(lightDirection),
    aIntensity(ambientIntensity),
    dIntensity(diffuseIntensity)
{}

Light::~Light()
{}

void Light::useLight(GLuint uniColor, GLuint uniDirection,
              GLuint uniAmbientIntensity, GLuint uniDiffuseIntensity)
{
    glUniform3f(uniColor, color.x, color.y, color.z);
    glUniform1f(uniAmbientIntensity, aIntensity);
}
