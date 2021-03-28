#version 330

in vec2 texCoord;
in vec3 normal;
in vec3 worldPosition;

out vec4 colour;

struct Light
{
    vec3 color;
    vec3 direction;
    float ambientIntensity;
    float diffuseIntensity;
};

struct Material
{
    float shininess;
    vec3 diffuseColor;
};

uniform sampler2D texSampler;
uniform Light light;
uniform Material material;
uniform vec3 cameraPosition;

void main()
{
    vec4 ambientColor = vec4(light.color * light.ambientIntensity, 1.0f);
    vec4 matColor = vec4(material.diffuseColor, 1.0f);
    // light direction is normalized at creation
    float diffuseFactor = max(-1*dot(normalize(normal),light.direction),0.0f);
    vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0);
    
    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (diffuseFactor > 0.0f)
    {
        vec3 dirToCamera = normalize(cameraPosition - worldPosition);
        vec3 reflectedLight = reflect(light.direction,normalize(normal));
        float specularFactor = dot(reflectedLight,dirToCamera);
        if (specularFactor > 0.0f)
        {
            specularColor = vec4(light.color * pow(specularFactor,material.shininess), 1.0f);
        }
    }
    
    colour = matColor * texture(texSampler, texCoord) * (ambientColor + diffuseColor + specularColor);
}
