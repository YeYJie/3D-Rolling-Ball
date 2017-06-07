#version 330 core

in vec2 textCoords;

out vec4 color;

uniform sampler2D origin;
uniform sampler2D bloom;

uniform float exposure = 0.8;

void main()
{
	vec3 hdrColor = texture(origin, textCoords).rgb + texture(bloom, textCoords).rgb;

	// exposure
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

	// gamma correct
	const float gamma = 1.0;
	result = pow(result, vec3(1.0 / gamma));

	color = vec4(result, 1.0f);
}