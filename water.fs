#version 330 core

in vec2 textCoords;
in vec4 clipSpace;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;

const float wave = 0.005f;

uniform float distortionOffset;

void main()
{

	vec2 temp = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;

	vec2 reflectionCoords = vec2(temp.x, -temp.y);
	vec2 refractionCoords = vec2(temp.x, temp.y);

	vec2 distortion1 = (texture(dudvTexture, 
		vec2(textCoords.x + distortionOffset, textCoords.y))).xy * 2.0f - 1.0f;
	vec2 distortion2 = (texture(dudvTexture,
		vec2(-textCoords.x + distortionOffset, -textCoords.y + distortionOffset))).xy * 2.0f - 1.0f;

	// vec2 distortion = texture(dudvTexture, vec2(textCoords.x, textCoords.y)).rg * 2.0f - 1.0f;
	vec2 distortion = distortion1 + distortion2;

	// vec2 distortedTexCoords = texture(dudvTexture, vec2(textCoords.x + distortionOffset, textCoords.y)).rg * 0.1;
	// distortedTexCoords = textCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + distortionOffset);
	// vec2 totalDistortion = (texture(dudvTexture, distortedTexCoords).rg * 2.0 - 1.0) * wave;

	reflectionCoords += distortion * wave;
	// reflectionCoords = clamp(reflectionCoords, 0.001, 0.999);
	reflectionCoords.x = clamp(reflectionCoords.x, 0.001, 0.999);
	reflectionCoords.y = clamp(reflectionCoords.y, -0.999, -0.001);

	refractionCoords += distortion * wave;
	refractionCoords = clamp(refractionCoords, 0.001, 0.999);

	vec4 reflection = texture(reflectionTexture, reflectionCoords);
	vec4 refraction = texture(refractionTexture, refractionCoords);
	// vec4 refraction = texture(refractionTexture, textCoords);

	color = mix(reflection, refraction, 0.5);
	
	// color = vec4(0.0f, 0.3f, 0.7f, 1.0f);
	// color = refraction;
	// color = reflection;

	// color = texture(reflectionTexture, textCoords);;
	// color = texture(dudvTexture, textCoords);
}