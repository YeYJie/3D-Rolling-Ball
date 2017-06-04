#ifndef _ENTITYSHADER_H_
#define _ENTITYSHADER_H_

#include "include.h"
#include "shader.h"

#define MAX_POINT_LIGHTS 10

class EntityShader : public Shader
{

public:

	EntityShader(const char * vertexShaderFileName,
					const char * fragmentShaderFileName)
		: Shader(vertexShaderFileName, fragmentShaderFileName)
	{
		bindGL();

		_clipPlane = getUniformLocation("clipPlane");
		_viewPosition = getUniformLocation("viewPosition");
		_dirLight = getUniformLocation("dirLight");
		_numLights = getUniformLocation("numLights");

		for(int i = 0; i < MAX_POINT_LIGHTS; ++i) {
			string pointLight = "pointLight[" + to_string(i) + "]";
			_pointLight[i] = getUniformLocation(pointLight.c_str());
		}

		_shadowMap = getUniformLocation("shadowMap");
		_useShadow = getUniformLocation("useShadow");

		unbindGL();
	}

	void setClipPlane(const glm::vec4 & clipPlane) {
		setUniform4f(_clipPlane, clipPlane);
	}
	void setClipPlane(float x, float y, float z, float w) {
		setUniform4f(_clipPlane, x, y, z, w);
	}

	void setViewPosition(const glm::vec3 & viewPosition) {
		setUniform3f(_viewPosition, viewPosition);
	}
	void setViewPosition(float x, float y, float z) {
		setUniform3f(_viewPosition, x, y, z);
	}

	void setDirLight(const glm::vec3 & dirLight) {
		setUniform3f(_dirLight, dirLight);
	}
	void setDirLight(float x, float y, float z) {
		setUniform3f(_dirLight, x, y, z);
	}

	void setNumLights(int num) {
		setUniform1i(_numLights, num);
	}

	void setPointLight(int i, const glm::vec3 & pointLight) {
		setUniform3f(_pointLight[i], pointLight);
	}
	void setPointLight(int i, float x, float y, float z) {
		setUniform3f(_pointLight[i], x, y, z);
	}

	void setShadowMap(int value) {
		setUniform1i(_shadowMap, value);
	}

	void setUseShadow(int value) {
		setUniform1i(_useShadow, value);
	}

private:

	GLint _clipPlane;

	GLint _viewPosition;

	GLint _dirLight;


	GLint _pointLight[MAX_POINT_LIGHTS];
	GLint _numLights;

	GLint _shadowMap;
	GLint _useShadow;

};

#undef MAX_POINT_LIGHTS

#endif