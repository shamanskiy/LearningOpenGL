#include "Shader.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include "Config.h"

using namespace std;

namespace 
{
    void checkShader(GLuint shaderID)
    {
        GLint result = 0;
        GLchar eLog[1024] = { 0 };
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

        if (!result)
        {
            glGetShaderInfoLog(shaderID, sizeof(eLog), NULL, eLog);
            throw runtime_error("Shader error: " + string(eLog));
        }
    }

    void checkProgram(GLuint programID, GLenum whatToCheck)
    {
        GLint result = 0;
        GLchar eLog[1024] = { 0 };
        glGetProgramiv(programID, whatToCheck, &result);

        if (!result)
        {
            glGetProgramInfoLog(programID, sizeof(eLog), NULL, eLog);
            throw runtime_error("Shader error: " + string(eLog));
        }
    }
}

Shader::Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
    auto vertexStr = readShaderCode(vertexShaderFile);
    auto fragmentStr = readShaderCode(fragmentShaderFile);
    createShaders(vertexStr, fragmentStr);
}

Shader::~Shader()
{
    deleteShaders();
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

    checkShader(shader);

    glAttachShader(m_id,shader);
}

void Shader::linkShaders()
{
    glLinkProgram(m_id);
    checkProgram(m_id, GL_LINK_STATUS);
}

void Shader::validateShaders()
{
    // To validate a shader, we need a bound VAO
    GLuint validationVAO;
	glGenVertexArrays(1, &validationVAO);
    glBindVertexArray(validationVAO);

    glValidateProgram(m_id);
    checkProgram(m_id, GL_VALIDATE_STATUS);

    glDeleteVertexArrays(1, &validationVAO);
}

void Shader::deleteShaders()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}