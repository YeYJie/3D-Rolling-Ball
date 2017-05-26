#version 330 core

layout(location=0) in vec2 position;

out vec2 textCoords;

uniform mat4 projection;
uniform mat4 modelViewMatrix;

void main()
{
	textCoords = position / 2.0 + 0.5;
	textCoords.y = 1.0 - textCoords.y;
	gl_Position = projection * modelViewMatrix * vec4(position, 0.0, 1.0);
}