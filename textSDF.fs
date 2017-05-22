#version 330 core

in vec2 textCoords;

out vec4 color;

uniform vec3 textColor;

uniform sampler2D guiTexture;

void main(void) 
{
	// color = vec4(textColor, texture(guiTexture, textCoords).r);
	color = texture(guiTexture, textCoords);
	// color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}