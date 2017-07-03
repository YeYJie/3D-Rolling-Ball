#version 330 core

layout(location=0) in vec2 position;

out vec2 textCoords;

// uniform mat4 model;

void main(void) {

	// gl_Position = model * vec4(position, 0.0f, 1.0f);
	gl_Position = vec4(position, 0.f, 1.0);

	// textCoords = vec2((position.x + 1.0) / 2.0,
	// 					1.0 - (position.y + 1.0) / 2.0);
	textCoords = vec2((position.x + 1.0f) / 2.0f,
						(position.y + 1.0f) / 2.0f);
}