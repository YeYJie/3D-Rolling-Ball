#version 330 core

// in vec2 textCoords;
in vec4 clipSpace;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main()
{

	vec2 temp = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	// vec2 temp = clipSpace.xy / clipSpace.w;

	vec2 reflectionCoords = vec2(temp.x, -temp.y);
	// vec2 refractionCoords = vec2(textCoords.x, textCoords.y);
	vec2 refractionCoords = vec2(temp.x, temp.y);

	vec4 reflection = texture(reflectionTexture, reflectionCoords);
	vec4 refraction = texture(refractionTexture, refractionCoords);
	// vec4 refraction = texture(refractionTexture, textCoords);

	color = mix(reflection, refraction, 0.5);
	
	// color = vec4(0.0f, 0.3f, 0.7f, 1.0f);
	// color = refraction;
	// color = reflection;

	// color = texture(reflectionTexture, textCoords);;
	// color = texture(refractionTexture, textCoords);
}