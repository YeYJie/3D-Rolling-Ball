#ifndef _ENTITYSHADER_H_
#define _ENTITYSHADER_H_

#include "include.h"
#include "shader.h"
#include "light.h"

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
		_numLightsLocation = getUniformLocation("numLights");

		for(int i = 0; i < MAX_POINT_LIGHTS; ++i) {
			string base = "lights[" + to_string(i) + "]";

			string type = base + ".type";
			string lightOn = base + ".lightOn";
			string position = base + ".position";
			string ambient = base + ".ambient";
			string diffuse = base + ".diffuse";
			string specular = base + ".specular";
			string shininess = base + ".shininess";
			string direction = base	+ ".direction";

			_lightsLocation[i * 8 + 0] = getUniformLocation(type.c_str());
			_lightsLocation[i * 8 + 1] = getUniformLocation(lightOn.c_str());
			_lightsLocation[i * 8 + 2] = getUniformLocation(position.c_str());
			_lightsLocation[i * 8 + 3] = getUniformLocation(ambient.c_str());
			_lightsLocation[i * 8 + 4] = getUniformLocation(diffuse.c_str());
			_lightsLocation[i * 8 + 5] = getUniformLocation(specular.c_str());
			_lightsLocation[i * 8 + 6] = getUniformLocation(shininess.c_str());
			_lightsLocation[i * 8 + 7] = getUniformLocation(direction.c_str());
		}

		_shadowMap = getUniformLocation("shadowMap");
		_useShadow = getUniformLocation("useShadow");

		unbindGL();
	}

	void resetLight() {
		setViewPosition(0, 0, 0);
		setNumLights(0);
		setUseShadow(0);
		_lights.clear();
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

	void setNumLights(int num) {
		setUniform1i(_numLightsLocation, num);
	}

	void setLight(int i, const LightPtr & light) {
		setUniform1i(_lightsLocation[i * 8 + 0], light->getType());
		setUniform1i(_lightsLocation[i * 8 + 1], light->getLightOn());
		setUniform3f(_lightsLocation[i * 8 + 2], light->getPosition());
		setUniform3f(_lightsLocation[i * 8 + 3], light->getAmbient());
		setUniform3f(_lightsLocation[i * 8 + 4], light->getDiffuse());
		setUniform3f(_lightsLocation[i * 8 + 5], light->getSpecular());
		setUniform1f(_lightsLocation[i * 8 + 6], light->getShininess());
		setUniform3f(_lightsLocation[i * 8 + 7], light->getDirection());
	}

	void addLight(LightPtr light) {
		light->increaseShaderRenfence();
		_lights.push_back(light);
	}

	void setShadowMap(int value) {
		setUniform1i(_shadowMap, value);
	}

	void setUseShadow(int value) {
		setUniform1i(_useShadow, value);
	}

	void updateLight() {
		int dirty = 0;
		for(int i = 0; i < _lights.size(); ++i)
		{
			if(_lights[i]->dirty()) {
				++dirty;
				setLight(i, _lights[i]);
				_lights[i]->decreaseDirty();
			}
		}
		if(dirty)
			setNumLights(_lights.size());
	}

private:

	GLint _clipPlane;

	GLint _viewPosition;

	GLint _lightsLocation[MAX_POINT_LIGHTS * 8]; // struct light has 7 fields

	GLint _numLightsLocation; 	// fragment shader location

	vector<LightPtr> _lights;

	GLint _shadowMap;
	GLint _useShadow;

};

#undef MAX_POINT_LIGHTS

#endif