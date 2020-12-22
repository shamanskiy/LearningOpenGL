#pragma once

#include <string>

#include <GL/glew.h>

// Shader is a class that manages creation and usage of OpenGL shader programs on a GPU
class Shader
{
private:
    // shader ID on GPU and IDs of uniform variables
    GLuint shaderID,
    uniModel, uniView, uniProjection,
    uniLightClr, uniLightDir, uniAmbientInt, uniDiffuseInt;

public:
    // default constructor, does not initialize anything on GPU
    Shader();
    ~Shader();

    // pass shader codes as C strings
    void createFromString(const std::string & vertexShaderCode,
                        const std::string & fragmentShaderCode);
    // read shader codes from files
    void createFromFile(const std::string & vertexShaderFilename,
                       const std::string & fragmentShaderFilename);

    // get IDs of uniform variables to set model, view and projection matrices
    GLuint uniModelMatrix() { return uniModel; }
    GLuint uniViewMatrix() { return uniView; }
    GLuint uniProjMatrix() { return uniProjection; }
    GLuint uniLightColor() { return uniLightClr; }
    GLuint uniLightDirection() { return uniLightDir; }
    GLuint uniAmbientIntensity() { return uniAmbientInt; }
    GLuint uniDiffuseIntensity() { return uniDiffuseInt; }

    // activate shader for further use
    void useShader() { glUseProgram(shaderID); }
    // free memory on GPU
    void deleteShader();

private:
    // compile shader on GPU
    void compileShader(const std::string & vShader,
                      const std::string & fShader);
    // add shader component
    void addShader(const std::string & shaderCode, GLenum shaderType);
    // read shader code from the file
    std::string readFile(const std::string & shaderCodeFilename);
};
