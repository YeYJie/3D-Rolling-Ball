#version 330 core

layout(location=0)in vec3 position;

// out vec2 textCoords;

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

	
	
	// textCoords = vec2((temp.x + 1.0f) / 2.0f, 
	// 					1.0f - (temp.z + 1.0f) / 2.0f);

	// vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	// clipSpace = projectionMatrix * viewMatrix * worldPosition;
	// gl_Position = clipSpace;
 // 	textureCoords = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5) * tiling;
 // 	toCameraVector = cameraPosition - worldPosition.xyz;
 // 	fromLightVector = worldPosition.xyz - lightPosition;
 	

 	// textCoords = vec2(position.x / 2000.0 + 0.5, position.z / 2000.0 + 0.5) * 4.0;

 	clipSpace = temp;

}