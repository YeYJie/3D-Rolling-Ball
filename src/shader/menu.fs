#version 330 core

in vec2 textCoords;

out vec4 color;

uniform sampler2D guiTexture;

const float offset = 1.0 / 80;  

void main()
{
	vec2 offsets[25] = vec2[](
		vec2(-2*offset, 2*offset),
		vec2(-offset, 2*offset),
		vec2(0, 2*offset),
		vec2(offset, 2*offset),
		vec2(2*offset, 2*offset),

		vec2(-2*offset, offset),
		vec2(-offset, offset),
		vec2(0.0f,    offset),
		vec2(offset,  offset),
		vec2(2*offset, offset),

		vec2(-2*offset, 0.0),
		vec2(-offset, 0.0f),
		vec2(0.0f,    0.0f),
		vec2(offset,  0.0f),
		vec2(2*offset, 0.0),

		vec2(-2*offset, -offset),
		vec2(-offset, -offset),
		vec2(0.0f,    -offset),
		vec2(offset,  -offset),
		vec2(2*offset, -offset),

		vec2(-2*offset, -2*offset),
		vec2(-offset, -2*offset),
		vec2(0, -2*offset),
		vec2(offset, -2*offset),
		vec2(2*offset, -2*offset)
	);

	float kernel[25] = float[](

		0.039911, 0.039978, 0.04, 0.039978, 0.039911, 
		0.039978, 0.040044, 0.040067, 0.040044, 0.039978, 
		0.04, 0.040067, 0.040089, 0.040067, 0.04, 
		0.039978, 0.040044, 0.040067, 0.040044, 0.039978, 
		0.039911, 0.039978, 0.04, 0.039978, 0.039911

		// 0.110741, 0.111296, 0.110741,
		// 0.111296, 0.111853, 0.111296,
		// 0.110741, 0.111296, 0.110741
		// 1.0 / 16.0 , 2.0 / 16.0 , 1.0 / 16.0 ,
		// 2.0 / 16.0 , 4.0 / 16.0 , 2.0 / 16.0 ,
		// 1.0 / 16.0 , 2.0 / 16.0 , 1.0 / 16.0
	);

	vec3 sampleTex[25];
	for(int i = 0; i < 25; i++)
	{
		vec2 temp = textCoords + offsets[i];
		temp = clamp(temp, 0.001, 0.999);
		sampleTex[i] = vec3(texture(guiTexture, temp));
		// sampleTex[i] = vec3(texture(guiTexture, textCoords + offsets[i]));
	}
	vec3 col = vec3(0.0f);
	for(int i = 0; i < 25; i++)
		col += sampleTex[i] * kernel[i];

	color = vec4(col, 1.0);
}