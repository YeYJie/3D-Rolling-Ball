#version 330 core

in vec2 textCoords;

out vec4 color;

uniform sampler2D guiTexture;

void main(void) {

	color = texture(guiTexture, vec2(textCoords.x, textCoords.y));

}