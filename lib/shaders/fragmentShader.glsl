#version 330

in vec2 posUV;
in vec3 normal;
in vec3 pos3D;

out vec4 color;

const int MAX_POINT_LIGHTS = 5;

struct AmbientLight
{
    vec3 color;
    float intensity;
};

struct DirectionalLight
{
    vec3 color;
    vec3 direction;
    float intensity;
};

struct PointLight
{
    vec3 color;
    vec3 position;
    vec3 attenuation;
    float intensity;
};

struct SpotLight
{
    vec3 color;
    vec3 attenuation;
    float intensity;
    float halfAngle;
    bool isOn;
};

struct Material
{
    float shininess;
    vec3 diffuseColor;
};

struct Camera
{
    vec3 position;
    vec3 direction;
};

uniform sampler2D texSampler;
uniform Camera camera;

uniform AmbientLight lightA;
uniform DirectionalLight lightD;
uniform int numPointLights;
uniform PointLight lightP[5];
uniform SpotLight lightS;

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
        vec3 dirToCamera = normalize(camera.position - pos3D);
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
    vec3 pointLightsColor = vec3(0.0,0.0,0.0);

    for (int i = 0; i < numPointLights; i++)
    {
        vec3 direction = pos3D - lightP[i].position;
        float distance = length(direction);
        float attenuation = computeAttenuation(lightP[i].attenuation, distance);
        pointLightsColor += computeLightFromDirection(lightP[i].color, normalize(direction),
                            lightP[i].intensity / attenuation);
    }

    return pointLightsColor;
}

void main()
{
    vec4 materialColor = vec4(material.diffuseColor, 1.0);
    vec4 lightColor = vec4(computeAmbientLight() + 
                      computeDirectionalLight() + 
                      computePointLights(), 1.0);

    color = materialColor * texture(texSampler, posUV) * lightColor;
}
