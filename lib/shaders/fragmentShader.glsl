#version 330

in vec2 posUV;
in vec3 normal;
in vec3 pos3D;

out vec4 color;

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
    vec3 attenuation;
    float intensity;
};

struct Material
{
    float shininess;
    vec3 diffuseColor;
};

uniform sampler2D texSampler;
uniform vec3 cameraPosition;

uniform LightAmbient lightA;
uniform LightDirectional lightD;
uniform LightPoint lightP;

uniform Material material;

vec3 computeAmbientLight()
{
    return lightA.color * lightA.intensity;
}

vec3 computeLightFromDirection(vec3 color, vec3 direction, float intensity)
{
    // Assume light direction is normalized
    vec3 normalizedNormal = normalize(normal);
    float incidentAngle = max(-1 * dot(normalizedNormal, direction), 0.0f);
    float lightFactor = intensity * incidentAngle;
    
    // Compute specular
    if (incidentAngle > 0.0f)
    {
        vec3 dirToCamera = normalize(cameraPosition - pos3D);
        vec3 reflectedLight = reflect(direction, normalizedNormal);
        float specularAngle = dot(reflectedLight, dirToCamera);
        if (specularAngle > 0.0f)
            lightFactor += pow(specularAngle,material.shininess);
    }

    return color * lightFactor;
}

vec3 computeDirectionalLight()
{
    return computeLightFromDirection(lightD.color, lightD.direction,
                                     lightD.intensity);
}

float computeAttenuation(vec3 coeffs, float dist)
{
    return (coeffs.x * dist + coeffs.y) * dist + coeffs.z;
}

vec3 computePointLights()
{
    vec3 direction = pos3D - lightP.position;
    float distance = length(direction);
    float attenuation = computeAttenuation(lightP.attenuation, distance);

    return computeLightFromDirection(lightP.color, normalize(direction),
        lightP.intensity / attenuation);
}

void main()
{
    vec4 materialColor = vec4(material.diffuseColor, 1.0);
    vec4 lightColor = vec4(computeAmbientLight() + 
                      computeDirectionalLight() + 
                      computePointLights(), 1.0);

    color = materialColor * texture(texSampler, posUV) * lightColor;
}
