#include "Light.h"

Light::Light() :
    color(glm::vec3(1.0f,1.0f,1.0f)),
    aIntensity(1.0f)
{}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity) :
    color(glm::vec3(red,green,blue)),
    aIntensity(ambientIntensity)
{}

Light::~Light()
{}

void Light::useLight(GLuint uniAmbientColor, GLuint uniAmbientIntensity)
{
    glUniform3f(uniAmbientColor, color.x, color.y, color.z);
    glUniform1f(uniAmbientIntensity, aIntensity);
}
