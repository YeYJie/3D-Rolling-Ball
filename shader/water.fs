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

const float wave = 0.05f;

uniform float distortionOffset;

in vec3 FragPos;
uniform vec3 lightPosition = vec3(10000.0f, 10000.0f, 10000.0f);
uniform vec3 viewPosition;

// uniform vec3 shitC;

void main()
{

	vec2 temp = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;

	vec2 reflectionCoords = vec2(temp.x, -temp.y);
	vec2 refractionCoords = vec2(temp.x, temp.y);

	// vec2 distortion1 = (texture(dudvTexture, 
	// 	vec2(textCoords.x + distortionOffset, textCoords.y))).xy * 2.0f - 1.0f;
	// vec2 distortion2 = (texture(dudvTexture,
	// 	vec2(-textCoords.x + distortionOffset, -textCoords.y + distortionOffset))).xy * 2.0f - 1.0f;

	// // vec2 distortion = texture(dudvTexture, vec2(textCoords.x, textCoords.y)).rg * 2.0f - 1.0f;
	// vec2 distortion = distortion1 + distortion2;
	// distortion *= wave;

	vec2 distortedTexCoords = texture(dudvTexture, vec2(textCoords.x + distortionOffset, textCoords.y)).rg * 0.1;
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
	

	vec4 normalColor = texture(normalMap, distortedTexCoords * 0.5f);
	vec3 normal = vec3(normalColor.r * 2.0f - 1.0f,
						normalColor.b * 5.0f,
						normalColor.g  * 2.0f - 1.0f);
	normal = normalize(normal);

	// vec3 norm = normalize(Normal);
	// vec3 viewDir = normalize(viewPosition - FragPos);
	// vec4 dir = CalcDirLight(dirLight, norm, viewDir);

	// color = dir;

	// color = texture(normalMap, distortedTexCoords * 0.1f);
	vec3 fromLightVector = FragPos - lightPosition;
	vec3 toCameraVector = viewPosition - FragPos;

	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(normalize(reflectedLight), normalize(toCameraVector)), 0.0);
	specular = pow(specular, 2.0f);
	vec3 specularHighlights = vec3(1.5f, 1.5f, 1.5f) * specular * 0.5;
	// vec3 specularHighlights = vec3(specular, specular, specular);

	// color = vec4(normalize(toCameraVector), 1.0f);
	// color = vec4(specularHighlights, 1.0f);
	color += vec4(specularHighlights, 1.0f);
	// color = normalColor;
}