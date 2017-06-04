#ifndef _SKYBOXSHADER_H_
#define _SKYBOXSHADER_H_

#include "include.h"
#include "shader.h"

class SkyboxShader : public Shader
{

public:

	SkyboxShader(const char * vertexShaderFileName,
					const char * fragmentShaderFileName)
		: Shader(vertexShaderFileName, fragmentShaderFileName)
	{
		bindGL();

		_cubeMapDay = getUniformLocation("cubeMapDay");
		_cubeMapNight = getUniformLocation("cubeMapNight");
		_blendFactor = getUniformLocation("blendFactor");

		unbindGL();
	}

	void setCubeMapDay(int value) {
		setUniform1i(_cubeMapDay, value);
	}

	void setCubeMapNight(int value) {
		setUniform1i(_cubeMapNight, value);
	}

	void setBlendFactor(float value) {
		setUniform1f(_blendFactor, value);
	}

private:

	GLint _cubeMapDay;
	GLint _cubeMapNight;

	GLint _blendFactor;

};


#endif