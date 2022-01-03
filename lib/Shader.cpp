#include "Shader.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include "Config.h"

using namespace std;

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
    //validateShaders();
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

void Shader::deleteShaders()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}