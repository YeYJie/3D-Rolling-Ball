#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 fs_textCoords;
in vec4 FragPosLightSpace;

out vec4 color;

uniform sampler2D text;
uniform sampler2D shadowMap;

uniform int useShadow = 0;

uniform vec3 viewPosition;

#define MAX_POINT_LIGHTS 10

uniform vec3 dirLight;
uniform vec3 pointLight[MAX_POINT_LIGHTS];
uniform int numLights = 0;

vec3 calculateDirLight(vec3 fromLight, vec3 normal, vec3 toViewer, vec3 textureColor);
vec3 calculatePointLight(vec3 lightPosition, vec3 fragmentPosition, vec3 normal, vec3 toViewer, vec3 textureColor);
float calculationShadow(vec4 fragPosLightSpace);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 toViewer = normalize(viewPosition - FragPos);

	vec4 textureColor = texture(text, fs_textCoords);

	vec3 res = calculateDirLight(dirLight, normal, toViewer, vec3(textureColor));

	for(int i = 0; i < numLights; ++i)
		res += calculatePointLight(pointLight[i], FragPos, normal, toViewer, vec3(textureColor));

	float shadowFactor = calculationShadow(FragPosLightSpace);
	if(useShadow == 1 && shadowFactor > 0.0)
	    color = vec4(0.3) * textureColor;
	else
		color = vec4(res, 1.0f);
}

vec3 calculateDirLight(vec3 fromLight, vec3 normal, vec3 toViewer, vec3 textureColor)
{
	vec3 ambient = vec3(0.3) * textureColor;

	vec3 toLight = normalize(-fromLight);

	float diffuseFactor = max(dot(toLight, normal), 0.0);
	vec3 diffuse = vec3(1.0) * diffuseFactor * textureColor;

	float specularFactor = pow(max(dot(reflect(fromLight, normal), toViewer), 0.0), 1.0);
	vec3 specular = vec3(0.3) * specularFactor * textureColor;

	return ambient + diffuse + specular;
}

vec3 calculatePointLight(vec3 lightPosition, vec3 fragmentPosition, vec3 normal, vec3 toViewer, vec3 textureColor)
{
	vec3 fromLight = normalize(fragmentPosition - lightPosition);
	vec3 toLight = normalize(-fromLight);

	float distanceToViewer = length(lightPosition - fragmentPosition);
	float attenuation = 1.0 / (1.0 + 0.007 * distanceToViewer + 0.0002 * (distanceToViewer * distanceToViewer));
	// float attenuation = 1.0;

	vec3 ambient = vec3(0.1) * textureColor * attenuation;

	float diffuseFactor = max(dot(toLight, normal), 0.0);
	vec3 diffuse = vec3(0.5) * diffuseFactor * textureColor * attenuation;

	float specularFactor = pow(max(dot(reflect(fromLight, normal), toViewer), 0.0), 1.0);
	vec3 specular = vec3(0.3) * specularFactor * textureColor * attenuation;

	return ambient + diffuse + specular;
}

float calculationShadow(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = currentDepth - 0.05 > closestDepth  ? 1.0 : 0.0;
	return shadow;
}