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
    float halfAngleCos;
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

uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLight;
uniform int numPointLights;
uniform PointLight pointLights[5];
uniform SpotLight spotLight;

uniform Material material;

vec3 computeAmbientLight()
{
    return ambientLight.color * ambientLight.intensity;
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
    if (directionalLight.intensity < 0)
        return vec3(0,0,0); 

    return computeLightFromDirection(directionalLight.color, directionalLight.direction,
                                     directionalLight.intensity);
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
        vec3 direction = pos3D - pointLights[i].position;
        float distance = length(direction);
        float attenuation = computeAttenuation(pointLights[i].attenuation, distance);
        pointLightsColor += computeLightFromDirection(pointLights[i].color, normalize(direction),
                            pointLights[i].intensity / attenuation);
    }

    return pointLightsColor;
}

vec3 computeSpotLight()
{
    if (!spotLight.isOn)
        return vec3(0.0,0.0,0.0);

    vec3 direction = pos3D - camera.position;
    direction.y += 0.3;
    float distance = length(direction);
    vec3 normalizedDirection = normalize(direction);
    float angleFromBeamAxis = dot(normalizedDirection, camera.direction);
    if (angleFromBeamAxis < spotLight.halfAngleCos)
        return vec3(0.0,0.0,0.0);

    float attenuation = computeAttenuation(spotLight.attenuation, distance);

    return computeLightFromDirection(spotLight.color,normalizedDirection,
        spotLight.intensity/attenuation) * (1.0 - (1.0 - angleFromBeamAxis)*(1.0f/(1.0f - spotLight.halfAngleCos)));
}

void main()
{
    vec4 materialColor = vec4(material.diffuseColor, 1.0);
    vec4 lightColor = vec4(computeAmbientLight() + 
                      computeDirectionalLight() + 
                      computePointLights() +
                      computeSpotLight(), 1.0);

    color = materialColor * texture(texSampler, posUV) * lightColor;
}
