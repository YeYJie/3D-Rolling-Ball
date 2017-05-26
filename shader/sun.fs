#version 330 core

in vec2 textCoords;

out vec4 color;

uniform sampler2D sunTexture;

uniform int renderSun;

uniform float bright;

void main(void){

    color = texture(sunTexture, textCoords);
    if(renderSun != 1)
    	color.a *= bright;
    // color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}