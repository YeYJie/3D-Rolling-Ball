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


uniform Material material = Material(vec3(0.5f), 
                                    vec3(1.0f), 
                                    vec3(0.5f), 
                                    32.0f);
uniform vec3 lightPosition = vec3(100.0f, 100.0f, 100.0f);

// uniform vec3 viewPosition = vec3(100.0f, 100.0f, 100.0f);
uniform vec3 viewPosition;

void main()
{
    // // Ambient
    vec3 ambient = material.ambient;

    // // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse;

    // // Specular
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = 0.01 * material.specular;

    vec4 lightColor = vec4(ambient + diffuse + specular, 1.0f);

    // color = vec4(result, 1.0f);
    // color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    color = texture(text, fs_textCoords) * lightColor;
    // color = texture(text, fs_textCoords);
}
