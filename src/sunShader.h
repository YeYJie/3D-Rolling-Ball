#ifndef _SUNSHADER_H_
#define _SUNSHADER_H_

#include "include.h"
#include "shader.h"

class SunShader : public Shader
{

public:

	SunShader(const char * vertexShaderFileName,
					const char * fragmentShaderFileName)
		: Shader(vertexShaderFileName, fragmentShaderFileName)
	{
		bindGL();

		_renderSun = getUniformLocation("renderSun");
		_modelViewMatrix = getUniformLocation("modelViewMatrix");
		_flareMatrix = getUniformLocation("flareMatrix");
		_sunTexture = getUniformLocation("sunTexture");
		_bright = getUniformLocation("bright");

		unbindGL();
	}

	void setRenderSun(int value) {
		setUniform1i(_renderSun, value);
	}

	void setModelViewMatrix(const glm::mat4 & m) {
		setUniformMatrix4fv(_modelViewMatrix, m);
	}

	void setFlareMatrix(const glm::mat4 & m) {
		setUniformMatrix4fv(_flareMatrix, m);
	}

	void setSuntexture(int value) {
		setUniform1i(_sunTexture, value);
	}

	void setBright(float value) {
		setUniform1f(_bright, value);
	}

private:

	GLint _renderSun;

	GLint _modelViewMatrix;

	GLint _flareMatrix;

	GLint _sunTexture;

	GLint _bright;

};


#endif