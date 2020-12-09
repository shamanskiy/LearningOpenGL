#version 330

in vec2 texCoord;

out vec4 colour;

struct Light
{
    vec3 color;
    vec3 direction;
    float ambientIntensity;
    float diffuseIntensity;
};

uniform sampler2D texSampler;
uniform Light light;

void main()
{
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    colour = texture(texSampler, texCoord) * ambientColor;
}
