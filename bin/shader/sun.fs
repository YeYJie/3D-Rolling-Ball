#version 330 core

in vec2 textCoords;

out vec4 color;

uniform sampler2D sunTexture;

uniform float bright;

void main(void){

    color = texture(sunTexture, textCoords);
    color.a *= bright;
}