#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 textCoords;
layout(location=2) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;
out vec2 TextCoords;

out float frag;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 clippingPlane;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	gl_ClipDistance[0] = dot(vec4(position, 1.0f), clippingPlane);

	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;

	TextCoords = textCoords;

	frag = position.y / 200.0;
	// frag2 = 1 - frag1;
}
