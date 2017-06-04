#ifndef _TERRAINSHADER_H_
#define _TERRAINSHADER_H_

#include "include.h"
#include "shader.h"

#define MAX_POINT_LIGHTS 10

class TerrainShader : public Shader
{

public:

	TerrainShader(const char * vertexShaderFileName,
					const char * fragmentShaderFileName)
		: Shader(vertexShaderFileName, fragmentShaderFileName)
	{
		bindGL();

		_clipPlane = getUniformLocation("clipPlane");
		_dirLight = getUniformLocation("dirLight");
		_numLights = getUniformLocation("numLights");

		for(int i = 0; i < MAX_POINT_LIGHTS; ++i) {
			string pointLight = "pointLight[" + to_string(i) + "]";
			_pointLight[i] = getUniformLocation(pointLight.c_str());
		}

		_text1 = getUniformLocation("text1");
		_text2 = getUniformLocation("text2");
		_shadowMap = getUniformLocation("shadowMap");

		unbindGL();
	}

	void setClipPlane(const glm::vec4 & clipPlane) {
		setUniform4f(_clipPlane, clipPlane);
	}
	void setClipPlane(float x, float y, float z, float w) {
		setUniform4f(_clipPlane, x, y, z, w);
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

	void setText(int i, int value) {
		switch(i) {
			case 1:
				setUniform1i(_text1, value); break;
			case 2:
				setUniform1i(_text2, value); break;
			default:
				cout << "\n\nterrainShader does not have text" << i << endl;
		}
	}

	void setShadowMap(int value) {
		setUniform1i(_shadowMap, value);
	}

private:

	GLint _clipPlane;

	GLint _dirLight;

	GLint _pointLight[MAX_POINT_LIGHTS];
	GLint _numLights;

	GLint _text1;
	GLint _text2;
	GLint _shadowMap;

};

#undef MAX_POINT_LIGHTS

#endif