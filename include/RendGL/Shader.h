#pragma once

#include <string>

#include <GL/glew.h>


// ShaderProgram loads shader glsl code and compiles it on the GPU.
// It then serves as a handle for the shader variables.
class ShaderProgram
{
public:
    ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
    ~ShaderProgram();

    void activateShader() const { glUseProgram(m_programID); }
    GLuint id() const { return m_programID; }
    
private:
    std::string readShaderCode(const std::string& filename);

    void createProgram(const std::string & vShader,
                      const std::string & fShader);
    // compile shader code on GPU
    void compileShader(const std::string & shaderCode, GLenum shaderType);

    void linkProgram();
    void validateProgram();
    
    // free memory on GPU
    void deleteProgram();
   
private:
    GLuint m_programID{0};
};
