#ifndef _WATERSHADER_H_
#define _WATERSHADER_H_

#include "include.h"
#include "shader.h"

class WaterShader : public Shader
{

public:

	WaterShader(const char * vertexShaderFileName,
					const char * fragmentShaderFileName)
		: Shader(vertexShaderFileName, fragmentShaderFileName)
	{
		bindGL();

		_distortionOffset = getUniformLocation("distortionOffset");
		_lightPosition = getUniformLocation("lightPosition");
		_viewPosition = getUniformLocation("viewPosition");

		_reflectionTexture = getUniformLocation("reflectionTexture");
		_refractionTexture = getUniformLocation("refractionTexture");
		_dudvTexture = getUniformLocation("dudvTexture");
		_normalMap = getUniformLocation("normalMap");

		unbindGL();
	}

	void setDistortionOffset(float d) {
		setUniform1f(_distortionOffset, d);
	}

	void setLightPosition(const glm::vec3 & lightPosition) {
		setUniform3f(_lightPosition, lightPosition);
	}
	void setLightPosition(float x, float y, float z) {
		setUniform3f(_lightPosition, x, y, z);
	}

	void setViewPosition(const glm::vec3 & viewPosition) {
		setUniform3f(_viewPosition, viewPosition);
	}
	void setViewPosition(float x, float y, float z) {
		setUniform3f(_viewPosition, x, y, z);
	}

	void setReflectionTexture(int value) {
		setUniform1i(_reflectionTexture, value);
	}

	void setRefractionTexture(int value) {
		setUniform1i(_refractionTexture, value);
	}

	void setDudvTexture(int value) {
		setUniform1i(_dudvTexture, value);
	}

	void setNormalMap(int value) {
		setUniform1i(_normalMap, value);
	}

private:

	GLuint _distortionOffset;
	GLuint _lightPosition;
	GLuint _viewPosition;

	GLuint _reflectionTexture;
	GLuint _refractionTexture;
	GLuint _dudvTexture;
	GLuint _normalMap;

};

#endif