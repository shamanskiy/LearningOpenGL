#version 330

in vec2 posUV;
in vec3 normal;
in vec3 pos3D;

out vec4 colour;

struct LightAmbient
{
    vec3 color;
    float intensity;
};

struct LightDirectional
{
    vec3 color;
    vec3 direction;
    float intensity;
};

struct LightPoint
{
    vec3 color;
    vec3 position;
    float intensity;
};

struct Material
{
    float shininess;
    vec3 diffuseColor;
};


uniform sampler2D texSampler;
uniform LightAmbient lightA;
uniform LightDirectional lightD;
uniform LightPoint lightP;
uniform Material material;
uniform vec3 cameraPosition;

void main()
{
    vec4 materialColor = vec4(material.diffuseColor, 1.0f);

    // Phong lighting = ambient + diffuse + specular
    vec4 phongColor = vec4(lightA.color * lightA.intensity, 1.0f);

    // Compute diffuse light
    // Light direction is normalized at creation
    float incidentAngle = max(-1*dot(normalize(normal),lightD.direction),0.0f);
    phongColor += vec4(lightD.color * lightD.intensity * incidentAngle, 1.0);
    
    // Compute specular light
    if (incidentAngle > 0.0f)
    {
        vec3 dirToCamera = normalize(cameraPosition - pos3D);
        vec3 reflectedLight = reflect(lightD.direction,normalize(normal));
        float specularAngle = dot(reflectedLight,dirToCamera);
        if (specularAngle > 0.0f)
            phongColor += vec4(lightD.color * pow(specularAngle,material.shininess), 1.0f);
    }
    
    colour = materialColor * texture(texSampler, posUV) * phongColor;
}
