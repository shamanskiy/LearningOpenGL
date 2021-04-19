#include "Shader.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include "Config.h"

using namespace std;

Shader::Shader()
{
    auto vertexStr = readShaderCode(SHADERS_DIR + "vertexShader.glsl");
    auto fragmentStr = readShaderCode(SHADERS_DIR + "fragmentShader.glsl");
    createShaders(vertexStr, fragmentStr);
}

Shader::~Shader()
{
    deleteShaders();
}

Shader::Shader(Shader&& other) noexcept :
    m_id(other.m_id),
    m_uniforms(other.m_uniforms)
{
    other.m_id = 0;
}

Shader& Shader::operator=(Shader&& other) & noexcept
{
    m_id = other.m_id;
    other.m_id = 0;
    m_uniforms = other.m_uniforms;
    return *this;
}

std::string Shader::readShaderCode(const std::string& filename)
{
    std::string fileContent;
    std::ifstream fileStream(filename);

    if (!fileStream.is_open())
        throw runtime_error("Failed to open " + filename);

    std::string line;
    while (getline(fileStream, line))
        fileContent.append(line + "\n");

    fileStream.close();
    return fileContent;
}

void Shader::createShaders(const std::string &vShader, const std::string &fShader)
{
    m_id = glCreateProgram();

    compileShader(vShader, GL_VERTEX_SHADER);
    compileShader(fShader, GL_FRAGMENT_SHADER);
    linkShaders();
    validateShaders();

    getUniforms();
}

void Shader::compileShader(const std::string &shaderCode, GLenum shaderType)
{
    auto shader = glCreateShader(shaderType);

    const char* codes[1];
    codes[0] = shaderCode.c_str();

    GLint codeLens[1];
    codeLens[0] = (GLint)strlen(codes[0]);

    glShaderSource(shader, 1, codes, codeLens);
    glCompileShader(shader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(shader,GL_COMPILE_STATUS,&result);
    if (!result)
    {
        glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
        throw runtime_error("Error compiling shader program: "
                                 + string(eLog));
    }

    glAttachShader(m_id,shader);
}

void Shader::linkShaders()
{
    glLinkProgram(m_id);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetProgramiv(m_id, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(m_id, sizeof(eLog), NULL, eLog);
        throw runtime_error("Error linking shader program: " +
                                 string(eLog));
    }
}

void Shader::validateShaders()
{
    glValidateProgram(m_id);
 
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(m_id, sizeof(eLog), NULL, eLog);
        throw runtime_error("Error validating shader program: " +
            string(eLog));
    }
}

void Shader::getUniforms()
{
    m_uniforms.cameraPosition = glGetUniformLocation(m_id, "cameraPosition");
    m_uniforms.modelMatrix = glGetUniformLocation(m_id, "model");
    m_uniforms.viewMatrix = glGetUniformLocation(m_id, "view");
    m_uniforms.projectionMatrix = glGetUniformLocation(m_id, "projection");

    m_uniforms.ambientLight.color = glGetUniformLocation(m_id, "lightA.color");
    m_uniforms.ambientLight.intensity = glGetUniformLocation(m_id, "lightA.intensity");

    m_uniforms.diffuseLight.color = glGetUniformLocation(m_id, "lightD.color");
    m_uniforms.diffuseLight.intensity = glGetUniformLocation(m_id, "lightD.intensity");
    m_uniforms.diffuseLight.direction = glGetUniformLocation(m_id, "lightD.direction");

    m_uniforms.numPointLights = glGetUniformLocation(m_id, "numPointLights");
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        m_uniforms.pointLights[i].color =
            glGetUniformLocation(m_id, (string("lightP[") + 
                to_string(i) + string("].color")).c_str());
        m_uniforms.pointLights[i].intensity =
            glGetUniformLocation(m_id, (string("lightP[") +
                to_string(i) + string("].intensity")).c_str());
        m_uniforms.pointLights[i].attenuation =
            glGetUniformLocation(m_id, (string("lightP[") +
                to_string(i) + string("].attenuation")).c_str());
        m_uniforms.pointLights[i].position =
            glGetUniformLocation(m_id, (string("lightP[") +
                to_string(i) + string("].position")).c_str());
    }

    m_uniforms.material.shininess = glGetUniformLocation(m_id, "material.shininess");
    m_uniforms.material.color = glGetUniformLocation(m_id, "material.diffuseColor");
}

void Shader::deleteShaders()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}