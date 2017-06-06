#version 330 core

in vec2 textCoords;
out vec4 color;

uniform sampler2D text;

uniform bool horizontal;

#define GAUSS_KERNEL_SIZE 7

uniform float weight[GAUSS_KERNEL_SIZE] = float[] (0.0783, 0.0782, 0.0779, 0.0774, 0.0767, 0.0759, 0.0748);

void main()
{
	vec2 text_offset = 1.0 / textureSize(text, 0);
	vec3 result = texture(text, textCoords).rgb * weight[0];

	if(horizontal)
	{
		for(int i = 1; i < GAUSS_KERNEL_SIZE; ++i)
		{
			result += texture(text, textCoords + vec2(text_offset.x * i, 0.0)).rgb * weight[i];
			result += texture(text, textCoords - vec2(text_offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for(int i = 1; i < GAUSS_KERNEL_SIZE; ++i)
		{
			result += texture(text, textCoords + vec2(0.0, text_offset.y * i)).rgb * weight[i];
			result += texture(text, textCoords - vec2(0.0, text_offset.y * i)).rgb * weight[i];
		}
	}

	color = vec4(result, 1.0);
}