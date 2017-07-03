#version 330 core

#define TEXT_MODE_NORMAL 	1
#define TEXT_MODE_OUTLINE 	2
#define TEXT_MODE_GLOW 		3
#define TEXT_MODE_DROPSHADOW 4

uniform int textMode = TEXT_MODE_NORMAL;

uniform vec3 textColor = vec3(1.0, 1.0, 0.0); // default yellow
uniform vec3 outlineColor = vec3(1.0, 0.0, 0.0); // default red
uniform vec3 glowColor = vec3(0.0, 1.0, 0.0); // default green
uniform vec3 dropShadowColor = vec3(0.0); // default black

in vec2 textCoords;
out vec4 color;

uniform sampler2D guiTexture;

const float normalWidth = 0.5;
const float normalEdge = 0.1;

const float outlineWidth = 0.65;
const float outlineEdge = 0.1;

const float glowWidth = 0.4;
const float glowEdge = 0.5;

const float dropShadowWidth = 0.4;
const float dropShadowEdge = 0.5;
const vec2 dropShadowOffset = vec2(-0.004);

void main()
{
	if(textMode == TEXT_MODE_NORMAL)
	{
		float normalDistance = 1.0 - texture(guiTexture, textCoords).a;
		float normalAlpha = 1.0 - smoothstep(normalWidth, normalWidth + normalEdge, normalDistance);

		color = vec4(textColor, normalAlpha);
	}
	else if(textMode == TEXT_MODE_OUTLINE)
	{
		float normalDistance = 1.0 - texture(guiTexture, textCoords).a;
		float normalAlpha = 1.0 - smoothstep(normalWidth, normalWidth + normalEdge, normalDistance);

		float outlineDistance = 1.0 - texture(guiTexture, textCoords).a;
		float outlineAlpha = 1.0 - smoothstep(outlineWidth, outlineWidth + outlineEdge, outlineDistance);

		float overallAlpha = normalAlpha + (1.0 - normalAlpha) * outlineAlpha;
		vec3 overallColor = mix(outlineColor, textColor, normalAlpha / overallAlpha);

		color = vec4(overallColor, overallAlpha);
	}
	else if(textMode == TEXT_MODE_GLOW)
	{
		float normalDistance = 1.0 - texture(guiTexture, textCoords).a;
		float normalAlpha = 1.0 - smoothstep(normalWidth, normalWidth + normalEdge, normalDistance);

		float glowDistance = 1.0 - texture(guiTexture, textCoords).a;
		float glowAlpha = 1.0 - smoothstep(glowWidth, glowWidth + glowEdge, glowDistance);

		float overallAlpha = normalAlpha + (1.0 - normalAlpha) * glowAlpha;
		vec3 overallColor = mix(glowColor, textColor, normalAlpha / overallAlpha);

		color = vec4(overallColor, overallAlpha);
	}
	else if(textMode == TEXT_MODE_DROPSHADOW)
	{
		float normalDistance = 1.0 - texture(guiTexture, textCoords).a;
		float normalAlpha = 1.0 - smoothstep(normalWidth, normalWidth + normalEdge, normalDistance);

		float dropShadowDistance = 1.0 - texture(guiTexture, textCoords + dropShadowOffset).a;
		float dropShadowAlpha = 1.0 - smoothstep(dropShadowWidth, dropShadowWidth + dropShadowEdge, dropShadowDistance);

		float overallAlpha = normalAlpha + (1.0 - normalAlpha) * dropShadowAlpha;
		vec3 overallColor = mix(dropShadowColor, textColor, normalAlpha / overallAlpha);

		color = vec4(overallColor, overallAlpha);
	}
}