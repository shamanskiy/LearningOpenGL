#version 330

in vec2 texCoord;
in vec3 normal;

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

    // light direction is normalized at creation
    float diffuseFactor = max(-1*dot(normalize(normal),light.direction),0.0f);
    vec4 diffuseColor = vec4(light.color,1.0f) * light.diffuseIntensity * diffuseFactor;
    
    colour = texture(texSampler, texCoord) * (ambientColor + diffuseColor);
}
