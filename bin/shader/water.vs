#version 330 core

layout(location=0)in vec3 position;

out vec2 textCoords;

out vec4 clipSpace;
out vec3 toCameraVector;
out vec3 fromLightVector;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


// out vec3 Normal;
out vec3 FragPos;

void main(void) {

	vec4 worldPosition = model * vec4(position, 1.0f);
	vec4 temp = projection * view * worldPosition;
	// vec4 temp = model * vec4()

	gl_Position = temp;

 	textCoords = vec2(position.x / 2.0 + 0.5, position.z / 2.0 + 0.5) * 5.0;

 	clipSpace = temp;

 	FragPos = vec3(worldPosition);
}