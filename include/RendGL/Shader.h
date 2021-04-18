#pragma once

#include <string>

#include <GL/glew.h>

struct UniformVariables {
    GLuint cameraPosition{ 0 };
    GLuint modelMatrix{ 0 };
    GLuint viewMatrix{ 0 };
    GLuint projectionMatrix{ 0 };

    GLuint lightAmbientColor{ 0 };
    GLuint lightAmbientIntensity{ 0 };

    GLuint lightDiffuseColor{ 0 };
    GLuint lightDiffuseIntensity{ 0 };
    GLuint lightDiffuseDirection{ 0 };

    GLuint lightPointColor{ 0 };
    GLuint lightPointIntensity{ 0 };
    GLuint lightPointPosition{ 0 };
    GLuint lightPointAttenuation{ 0 };
    
    GLuint materialShininess{ 0 };
    GLuint materialColor{ 0 };
};


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

    const UniformVariables& uniforms() const { return m_uniforms; }
    void activateShader() const { glUseProgram(m_id); }
    
private:
    std::string readShaderCode(const std::string& filename);

    void createShaders(const std::string & vShader,
                      const std::string & fShader);
    // compile shader code on GPU
    void compileShader(const std::string & shaderCode, GLenum shaderType);

    void linkShaders();
    void validateShaders();
    void getUniforms();

    // free memory on GPU
    void deleteShaders();
   
private:
    GLuint m_id{0};
    // uniform variables
    UniformVariables m_uniforms;
};
