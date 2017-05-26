#version 330 core

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

in float frag;

out vec4 color;

uniform vec3 viewPos;
uniform sampler2D text1;
uniform sampler2D text2;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 temp = vec3(mix(texture(text1, TextCoords), texture(text2, TextCoords), frag));


	vec3 ambient = light.ambient * temp;
	vec3 diffuse = light.diffuse * diff * temp;
	return (ambient + diffuse);
}

void main()
{
	DirLight dirLight;
	dirLight.direction = vec3(-0.2f, -1.0f, -0.1f);
	dirLight.ambient = vec3(0.4f, 0.4f, 0.4f);
	dirLight.diffuse = vec3(0.5f, 0.5f, 0.5f);
	dirLight.specular = vec3(0.5f, 0.5f, 0.5f);

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	color = vec4(CalcDirLight(dirLight, norm, viewDir), 1.0f);
}