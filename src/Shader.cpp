#include "Shader.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

Shader::Shader() :
    shaderID(0),
    uniModel(0),
    uniProjection(0)
{ }

Shader::~Shader()
{
    deleteShader();
}

void Shader::createFromString(const std::string &vertexShader, const std::string &fragmentShader)
{
    compileShader(vertexShader, fragmentShader);
}

void Shader::createFromFile(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
{
    std::string vertexStr = readFile(vertexShaderFilename);
    std::string fragmentStr = readFile(fragmentShaderFilename);
    compileShader(vertexStr, fragmentStr);
}

void Shader::deleteShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    
    uniModel = 0;
    uniProjection = 0;
}

void Shader::compileShader(const std::string &vShader, const std::string &fShader)
{
    shaderID = glCreateProgram();
    
    if (!shaderID)
    {
        std::cout << "Error creating shader program\n";
        return;
    }
    
    addShader(vShader, GL_VERTEX_SHADER);
    addShader(fShader, GL_FRAGMENT_SHADER);
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking shader program: " << eLog << std::endl;
        return;
    }
    
    uniModel = glGetUniformLocation(shaderID,"model");
    uniView = glGetUniformLocation(shaderID,"view");
    uniProjection = glGetUniformLocation(shaderID,"projection");
    
    //========== Validation ===//
    
    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout << "Error validating shader program: " << eLog << std::endl;
        return;
    }
    glBindVertexArray(0);
}

void Shader::addShader(const std::string &shaderCode, GLenum shaderType)
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
    
    glAttachShader(shaderID,shader);
}

std::string Shader::readFile(const std::string &shaderCodeFilename)
{
    std::string fileContent;
    std::ifstream fileStream(shaderCodeFilename,std::ios::in);
    
    if (!fileStream.is_open())
    {
        std::cout << "File " << shaderCodeFilename << " does not exist\n";
        return "";
    }
    
    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream,line);
        fileContent.append(line + "\n");
    }
    
    fileStream.close();
    return fileContent;
}
