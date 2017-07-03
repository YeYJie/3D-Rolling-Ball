#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;
in vec4 FragPosLightSpace;

in float frag;

// out vec4 color;
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 brightColor;

in vec2 worldxz;

uniform sampler2D text1;
uniform sampler2D text2;
uniform sampler2D shadowMap;

uniform int useShadow = 0;


#define MAX_POINT_LIGHTS    10
#define DIRECTION_LIGHT     0
#define POINT_LIGHT         1

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


vec3 calculateDirLight(Light light, vec3 normal, vec3 textureColor, float shadowFactor);
vec3 calculatePointLight(Light light, vec3 fragmentPosition, vec3 normal, vec3 textureColor, float shadowFactor);
float calculationShadow(vec4 fragPosLightSpace);


void main()
{
	vec3 normal = normalize(Normal);

	vec4 textureColor = mix(texture(text2, TextCoords), texture(text1, TextCoords), frag);
	// vec4 textureColor = vec4(1.0);

	vec3 res = vec3(0.0);

	float shadowFactor = 0.0;
	if(useShadow == 1)
		shadowFactor = calculationShadow(FragPosLightSpace);

	for(int i = 0; i < numLights; ++i)
	{
		if(lights[i].lightOn)
		{
			if(lights[i].type == DIRECTION_LIGHT)
				res += calculateDirLight(lights[i], normal, vec3(textureColor), shadowFactor);
			else
				res += calculatePointLight(lights[i], FragPos, normal, vec3(textureColor), shadowFactor);
		}
	}

	color = vec4(res, 1.0f);

	brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}


vec3 calculateDirLight(Light light, vec3 normal, vec3 textureColor, float shadowFactor)
{
	vec3 ambient = light.ambient * textureColor;

	vec3 toLight = normalize(-light.direction);

	float diffuseFactor = max(dot(toLight, normal), 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * textureColor;

	return ambient + (1.0 - shadowFactor) * diffuse;
}

float constant = 1.0;
float linear = 0.007;
float quadratic = 0.0002;

vec3 calculatePointLight(Light light, vec3 fragmentPosition, vec3 normal, vec3 textureColor, float shadowFactor)
{
	vec3 fromLight = normalize(fragmentPosition - light.position);
	vec3 toLight = normalize(-fromLight);

	float distanceToViewer = length(light.position - fragmentPosition);
	float attenuation = 1.0f / (constant + linear*distanceToViewer + quadratic*(distanceToViewer*distanceToViewer));

	vec3 ambient = light.ambient * textureColor * attenuation;

	float diffuseFactor = max(dot(toLight, normal), 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * textureColor * attenuation;

	return ambient + (1.0 - shadowFactor) * diffuse;
}


float calculationShadow(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0)
		return 0.0;

	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -2; x <= 2; ++x)
	{
		for(int y = -2; y <= 2; ++y)
		{
			float pcf = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - 0.005 > pcf ? 1.0 : 0.0;
		}
	}
	return shadow / 25.0;
}