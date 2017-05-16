#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 FragPos;
in vec3 Normal;

in vec2 fs_textCoords;

out vec4 color;

uniform sampler2D text;

uniform Material material;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
    // Ambient
    vec3 ambient = material.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse;
    // vec3 diffuse = material.diffuse;

    // Specular
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular;
    // vec3 specular = material.specular;

    vec3 result = ambient + diffuse + specular;
    // color = vec4(result, 1.0f);
    // color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    color = texture(text, fs_textCoords);
}
