#version 330

in vec2 posUV;
in vec3 normal;
in vec3 pos3D;

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
    vec4 materialColor = vec4(material.diffuseColor, 1.0f);

    // Phong lighting = ambient + diffuse + specular
    vec4 phongColor = vec4(light.color * light.ambientIntensity, 1.0f);

    // Compute diffuse light
    // Light direction is normalized at creation
    float incidentAngle = max(-1*dot(normalize(normal),light.direction),0.0f);
    phongColor += vec4(light.color * light.diffuseIntensity * incidentAngle, 1.0);
    
    // Compute specular light
    if (incidentAngle > 0.0f)
    {
        vec3 dirToCamera = normalize(cameraPosition - pos3D);
        vec3 reflectedLight = reflect(light.direction,normalize(normal));
        float specularAngle = dot(reflectedLight,dirToCamera);
        if (specularAngle > 0.0f)
            phongColor += vec4(light.color * pow(specularAngle,material.shininess), 1.0f);
    }
    
    colour = materialColor * texture(texSampler, posUV) * phongColor;
}
