#pragma once

#include <string>

#include <GL/glew.h>


// Shader loads shader glsl code and compiles it on the GPU.
// It then serves as a handle for the shader variables.
class Shader
{
public:
    // move-only
    Shader();
    ~Shader();
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) & noexcept;

    void activateShader() const { glUseProgram(m_id); }
    GLuint id() const { return m_id; }
    
private:
    std::string readShaderCode(const std::string& filename);

    void createShaders(const std::string & vShader,
                      const std::string & fShader);
    // compile shader code on GPU
    void compileShader(const std::string & shaderCode, GLenum shaderType);

    void linkShaders();
    void validateShaders();
    
    // free memory on GPU
    void deleteShaders();
   
private:
    GLuint m_id{0};
};
