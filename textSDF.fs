#version 330 core

in vec2 textCoords;

out vec4 color;

uniform vec3 textColor;

uniform sampler2D guiTexture;

const float width = 0.5;
const float edge = 0.01;

void main(void) 
{
	float distance = 1.0 - texture(guiTexture, textCoords).a;
	float alpha = 1.0 - smoothstep(width, width + edge, distance);

	color = vec4(textColor, alpha);
}