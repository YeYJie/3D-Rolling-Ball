#version 330 core

layout(location=0)in vec3 position;

out vec2 textCoords;

out vec4 clipSpace;
// out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;
// out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;



// const float tiling = 4.0;

// out vec3 Normal;
out vec3 FragPos;

void main(void) {

	vec4 worldPosition = model * vec4(position, 1.0f);
	vec4 temp = projection * view * worldPosition;
	// vec4 temp = model * vec4()

	gl_Position = temp;

 	textCoords = vec2(position.x / 2.0f + 0.5f, position.z / 2.0f + 0.5f) * 4.0f;

 	clipSpace = temp;

    // FragPos = vec3(model * vec4(position, 1.0f));
    // Normal = mat3(transpose(inverse(model))) * vec3(0.0f, 1.0f, 0.0f);
 	// toCameraVector = viewPosition - worldPosition.xyz;
 	// fromLightVector = worldPosition.xyz - lightPosition;

 	FragPos = vec3(worldPosition);
}