#version 400

in vec3 textureCoords;
out vec4 out_Color;

uniform samplerCube cubeMapDay;
uniform samplerCube cubeMapNight;

uniform float blendFactor = 0.0f;

void main(void){

    vec4 day = texture(cubeMapNight, textureCoords);
    vec4 night = texture(cubeMapNight, textureCoords);
    out_Color = mix(day, night, blendFactor);

}