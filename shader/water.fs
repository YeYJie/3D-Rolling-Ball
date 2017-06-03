#version 330 core

in vec2 textCoords;
in vec4 clipSpace;

in vec3 toCameraVector;
in vec3 fromLightVector;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;
uniform sampler2D normalMap;

const float wave = 0.01;

uniform float distortionOffset;

in vec3 FragPos;
uniform vec3 viewPosition;
uniform vec3 dirLight;

void main()
{
	vec2 temp = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;

	vec2 reflectionCoords = vec2(temp.x, -temp.y);
	vec2 refractionCoords = vec2(temp.x, temp.y);


	vec2 distortedTexCoords = texture(dudvTexture, vec2(textCoords.x + distortionOffset, textCoords.y)).rg * 0.01;
	distortedTexCoords = textCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + distortionOffset);
	vec2 distortion = (texture(dudvTexture, distortedTexCoords).rg * 2.0 - 1.0) * wave;

	reflectionCoords += distortion;
	reflectionCoords.x = clamp(reflectionCoords.x, 0.001, 0.999);
	reflectionCoords.y = clamp(reflectionCoords.y, -0.999, -0.001);

	refractionCoords += distortion;
	refractionCoords = clamp(refractionCoords, 0.001, 0.999);

	vec4 reflection = texture(reflectionTexture, reflectionCoords);
	vec4 refraction = texture(refractionTexture, refractionCoords);

	color = mix(reflection, refraction, 0.5);
	// color = reflection;

	vec4 normalColor = texture(normalMap, distortedTexCoords);
	vec3 normal = vec3(normalColor.r * 2.0f - 1.0f,
						normalColor.b * 3.0f,
						normalColor.g  * 2.0f - 1.0f);
	normal = normalize(normal);


	vec3 fromLight = normalize(dirLight);
	vec3 toCamera = normalize(viewPosition - FragPos);

	float specularFactor = pow(max(dot(reflect(fromLight, normal), toCamera), 0.0), 4.0);
	vec3 specular = vec3(1.2) * specularFactor * 0.2;

	color += vec4(specular, 1.0);
}