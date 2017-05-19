#version 330 core

layout(location=0)in vec3 position;

out vec2 textCoords;

out vec4 clipSpace;
// out vec2 textureCoords;
// out vec3 toCameraVector;
// out vec3 fromLightVector;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
// uniform vec3 lightPosition;

// uniform vec3 cameraPosition;

// const float tiling = 4.0;

void main(void) {

	vec4 temp = projection * view * model * vec4(position, 1.0f);
	// vec4 temp = model * vec4()

	gl_Position = temp;

 	textCoords = vec2(position.x / 2.0f + 0.5f, position.z / 2.0f + 0.5f) * 2.0f;

 	clipSpace = temp;

}