#version 330 core

layout(location=0) in vec2 position;
// layout(location=1) in vec2 in_textCoords;

out vec2 textCoords;

uniform mat4 model;

uniform mat3 textTranslateMatrix;

void main(void) {

	gl_Position = model * vec4(position, 0.0f, 1.0f);
	
	// textCoords = vec2((position.x + 1.0f) / 2.0f, 
	// 					1.0f - (position.y + 1.0f) / 2.0f);

	// mat3 shit;
	// shit[0][0] = 0.025391;
	// shit[2][0] = 0.052734;
	// shit[1][1] = 0.074219;
	// shit[2][1] = 0.925781;
	// shit[2][2] = 1.0;

	// textCoords = vec2(position.x, 1.0-position.y);
	textCoords = (textTranslateMatrix * vec3(position.x, position.y, 1.0f)).xy;
	textCoords.y = 1.0 - textCoords.y;

	// textCoords.x = 0.025391 * position.x + 0.052734;
	// textCoords.y = 0.074219 * position.y + 0.925781;
	// textCoords.y = 1.0 - textCoords.y;
}