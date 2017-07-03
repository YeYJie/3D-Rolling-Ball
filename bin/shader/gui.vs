#version 330 core

layout(location=0) in vec2 position;

out vec2 textCoords;

uniform mat4 model;

void main(void) {

	gl_Position = model * vec4(position, 0.0f, 1.0f);
	// textCoords = vec2((position.x + 1.0f) / 2.0f,
	// 					1.0f - (position.y + 1.0f) / 2.0f);
	textCoords = vec2((position.x + 1.0f) / 2.0f,
						(position.y + 1.0f) / 2.0f);
}