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

#define MAX_POINT_LIGHTS 	10
#define DIRECTION_LIGHT 	0
#define POINT_LIGHT 		1

struct Light
{
	int type;

	bool lightOn;

	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

	vec3 direction;
};

uniform int numLights = 0;
uniform Light lights[MAX_POINT_LIGHTS];

vec3 calculateDirLight(Light light, vec3 normal, vec3 toViewer, vec3 textureColor);
vec3 calculatePointLight(Light light, vec3 fragmentPosition, vec3 normal, vec3 toViewer, vec3 textureColor);
float calculationShadow(vec4 fragPosLightSpace);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 toViewer = normalize(viewPosition - FragPos);

	vec4 textureColor = texture(text, fs_textCoords);

	vec3 res = vec3(0.0);

	for(int i = 0; i < numLights; ++i)
	{
		if(lights[i].lightOn)
		{
			if(lights[i].type == DIRECTION_LIGHT)
				res += calculateDirLight(lights[i], normal, toViewer, vec3(textureColor));
			else
				res += calculatePointLight(lights[i], FragPos, normal, toViewer, vec3(textureColor));
		}
	}

	float shadowFactor = calculationShadow(FragPosLightSpace);
	if(useShadow == 1 && shadowFactor > 0.0)
	    color = vec4(0.3) * textureColor;
	else
		color = vec4(res, 1.0f);
}


vec3 calculateDirLight(Light light, vec3 normal, vec3 toViewer, vec3 textureColor)
{
	vec3 ambient = light.ambient * textureColor;

	vec3 toLight = normalize(-light.direction);

	float diffuseFactor = max(dot(toLight, normal), 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * textureColor;

	float specularFactor = pow(max(dot(reflect(light.direction, normal), toViewer), 0.0), light.shininess);
	vec3 specular = light.specular * specularFactor * textureColor;

	return ambient + diffuse + specular;
}


vec3 calculatePointLight(Light light, vec3 fragmentPosition, vec3 normal, vec3 toViewer, vec3 textureColor)
{
	vec3 fromLight = normalize(fragmentPosition - light.position);
	vec3 toLight = normalize(-fromLight);

	float distanceToViewer = length(light.position - fragmentPosition);
	float attenuation = 1.0 / (1.0 + 0.007 * distanceToViewer + 0.0002 * (distanceToViewer * distanceToViewer));
	// float attenuation = 1.0;

	vec3 ambient = light.ambient * textureColor * attenuation;

	float diffuseFactor = max(dot(toLight, normal), 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * textureColor * attenuation;

	float specularFactor = pow(max(dot(reflect(fromLight, normal), toViewer), 0.0), light.shininess);
	vec3 specular = light.specular * specularFactor * textureColor * attenuation;

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