#version 330 core

layout(location=0) in vec3 position;

out vec3 textureCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model = mat4(1.0f);

void main(void){
	
	gl_Position = projection * view * model * vec4(position, 1.0); 
	
	textureCoords = position;
	
}