#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

in float frag;

out vec4 color;

in vec2 worldxz;

uniform sampler2D text1;
uniform sampler2D text2;

#define MAX_POINT_LIGHTS 10

uniform vec3 dirLight;
uniform vec3 pointLight[MAX_POINT_LIGHTS];
uniform int numLights = 0;

vec3 calculateDirLight(vec3 fromLight, vec3 normal, vec3 textureColor);
vec3 calculatePointLight(vec3 lightPosition, vec3 fragmentPosition, vec3 normal, vec3 textureColor);

void main()
{
    vec3 normal = normalize(Normal);

    vec4 textureColor = mix(texture(text2, TextCoords), texture(text1, TextCoords), frag);

    vec3 res = calculateDirLight(dirLight, normal, vec3(textureColor));

    for(int i = 0; i < numLights; ++i)
        res += calculatePointLight(pointLight[i], FragPos, normal, vec3(textureColor));

    color = vec4(res, 1.0f);
}

vec3 calculateDirLight(vec3 fromLight, vec3 normal, vec3 textureColor)
{
    vec3 ambient = vec3(0.3) * textureColor;

    vec3 toLight = normalize(-fromLight);

    float diffuseFactor = max(dot(toLight, normal), 0.0);
    vec3 diffuse = vec3(0.7) * diffuseFactor * textureColor;

    return ambient + diffuse;
}

vec3 calculatePointLight(vec3 lightPosition, vec3 fragmentPosition, vec3 normal, vec3 textureColor)
{
    vec3 fromLight = normalize(fragmentPosition - lightPosition);
    vec3 toLight = normalize(-fromLight);

    float distanceToViewer = length(lightPosition - fragmentPosition);
    float attenuation = 1.0 / (1.0 + 0.007 * distanceToViewer + 0.0002 * (distanceToViewer * distanceToViewer));

    vec3 ambient = vec3(0.1) * textureColor * attenuation;

    float diffuseFactor = max(dot(toLight, normal), 0.0);
    vec3 diffuse = vec3(0.5) * diffuseFactor * textureColor * attenuation;

    return ambient + diffuse;
}