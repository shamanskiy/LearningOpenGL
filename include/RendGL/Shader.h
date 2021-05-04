#pragma once

#include <string>

#include <GL/glew.h>

const size_t MAX_POINT_LIGHTS = 5;

struct UniformVariables {
    GLuint modelMatrix{ 0 };
    GLuint viewMatrix{ 0 };
    GLuint projectionMatrix{ 0 };

    struct {
        GLuint position{ 0 };
        GLuint direction{ 0 };
    } camera;

    struct {
        GLuint color{ 0 };
        GLuint intensity{ 0 };
    } ambientLight;

    struct {
        GLuint color{ 0 };
        GLuint intensity{ 0 };
        GLuint direction{ 0 };
    } diffuseLight;

    struct {
        GLuint color{ 0 };
        GLuint intensity{ 0 };
        GLuint position{ 0 };
        GLuint attenuation{ 0 };
    } pointLights[MAX_POINT_LIGHTS];

    struct {
        GLuint color{ 0 };
        GLuint intensity{ 0 };
        GLuint attenuation{ 0 };
        GLuint halfAngleCos{ 0 };
        GLuint verticalOffset{ 0 };
        GLuint isOn{ 0 };
    } spotLight;

    GLuint numPointLights{ 0 };
    
    struct {
        GLuint shininess{ 0 };
        GLuint color{ 0 };
    } material;
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
