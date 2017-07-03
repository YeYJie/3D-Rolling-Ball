#version 330 core

layout(location=0) in vec2 position;

out vec2 textCoords;

uniform int renderSun;

uniform mat4 projection;
uniform mat4 modelViewMatrix;

// uniform vec2 flarePosition;
uniform mat4 flareMatrix;

void main()
{
	if(renderSun == 1) 
	{
		// textCoords = position / 2.0 + 0.5;
		// textCoords.y = 1.0 - textCoords.y;
		gl_Position = projection * modelViewMatrix * vec4(position, 0.0, 1.0);
	}
	else 
	{
		// gl_Position = vec4(
		// 						2.0 * flarePosition.x - 1.0, 
		// 						1.0 - 2.0 * flarePosition.y, 
		// 						0.0, 1.0
		// 				);
		gl_Position = flareMatrix * vec4(position, 0.0, 1.0);
	}

	textCoords = position / 2.0 + 0.5;
	textCoords.y = 1.0 - textCoords.y;
}