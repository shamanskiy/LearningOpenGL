#include "Shader.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include "Config.h"

Shader::Shader()
{
    auto vertexStr = readShaderCode(SHADERS_DIR + "vertexShader_texture.glsl");
    auto fragmentStr = readShaderCode(SHADERS_DIR + "fragmentShader_texture.glsl");
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
    {
        std::cout << "File " << filename << " does not exist\n";
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        fileContent.append(line + "\n");
    }

    fileStream.close();
    return fileContent;
}

void Shader::createShaders(const std::string &vShader, const std::string &fShader)
{
    m_id = glCreateProgram();

    if (!m_id)
    {
        std::cout << "Error creating shader program\n";
        return;
    }

    compileShader(vShader, GL_VERTEX_SHADER);
    compileShader(fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(m_id, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking shader program: " << eLog << std::endl;
        return;
    }

    m_uniforms.cameraPosition = glGetUniformLocation(m_id, "cameraPosition");
    m_uniforms.modelMatrix = glGetUniformLocation(m_id, "model");
    m_uniforms.viewMatrix = glGetUniformLocation(m_id, "view");
    m_uniforms.projectionMatrix = glGetUniformLocation(m_id, "projection");
    m_uniforms.lightColor = glGetUniformLocation(m_id, "light.color");
    m_uniforms.lightDirection = glGetUniformLocation(m_id, "light.direction");
    m_uniforms.ambientIntensity = glGetUniformLocation(m_id, "light.ambientIntensity");
    m_uniforms.diffuseIntensity = glGetUniformLocation(m_id, "light.diffuseIntensity");
    m_uniforms.materialShininess = glGetUniformLocation(m_id, "material.shininess");
    m_uniforms.specularIntensity = glGetUniformLocation(m_id, "material.specularIntensity");

    //========== Validation ===//

    glValidateProgram(m_id);
    glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(m_id, sizeof(eLog), NULL, eLog);
        std::cout << "Error validating shader program: " << eLog << std::endl;
        return;
    }
    glBindVertexArray(0);
}

void Shader::compileShader(const std::string &shaderCode, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

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
        std::cout << "Error compiling " << shaderType << " shader program: " << eLog << std::endl;
        return;
    }

    glAttachShader(m_id,shader);
}

void Shader::deleteShaders()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}