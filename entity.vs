#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textCoords;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

out vec2 fs_textCoords;

uniform vec4 clipPlane;

void main()
{
	gl_Position =  projection * view * model * vec4(position, 1);

	gl_ClipDistance[0] = dot(model * vec4(position, 1), clipPlane);

    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;

    fs_textCoords = textCoords;
}
