#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "include.h"
#include "entity.h"

#define DIRECTION_LIGHT 0
#define POINT_LIGHT 	1

class Light : public Entity
{

public:

	Light() {}

	Light(TexturedModelPtr model, glm::vec3 position,
			glm::vec3 rotation, float scale)
		: Entity(model, position, rotation, scale) { }

	Light(TexturedModelPtr model, glm::vec3 position,
			float rotX, float rotY, float rotZ, float scale)
		: Entity(model, position, rotX, rotY, rotZ, scale) { }

	Light(TexturedModelPtr model, GLuint textureIndex, glm::vec3 position,
			glm::vec3 rotation, float scale)
		: Entity(model, textureIndex, position, rotation, scale) { }

	Light(TexturedModelPtr model, GLuint textureIndex, glm::vec3 position,
			float rotX, float rotY, float rotZ, float scale)
		: Entity(model, textureIndex, position, rotX, rotY, rotZ, scale) { }


	int getType() const { return _type; }

	glm::vec3 getAmbient() const { return _ambient; }
	glm::vec3 getDiffuse() const { return _diffuse; }
	glm::vec3 getSpecular() const { return _specular; }
	float getShininess() const { return _shininess; }

	glm::vec3 getDirection() const { return _direction; }


	void setType(int type) { _type = type; makeDirty(); }

	void setAmbient(float ambient) { _ambient = glm::vec3(ambient); makeDirty(); }
	void setAmbient(float r, float g, float b) { _ambient = glm::vec3(r, g, b); makeDirty(); }
	void setAmbient(const glm::vec3 & ambient) { _ambient = ambient; makeDirty(); }

	void setDiffuse(float diffuse) { _diffuse = glm::vec3(diffuse); makeDirty(); }
	void setDiffuse(float r, float g, float b) { _diffuse = glm::vec3(r, g, b); makeDirty(); }
	void setDiffuse(const glm::vec3 & diffuse) { _diffuse = diffuse; makeDirty(); }

	void setSpecular(float specular) { _specular = glm::vec3(specular); makeDirty(); }
	void setSpecular(float r, float g, float b) { _specular = glm::vec3(r, g, b); makeDirty(); }
	void setSpecular(const glm::vec3 & specular) { _specular = specular; makeDirty(); }

	void setShininess(float shininess) { _shininess = shininess; makeDirty(); }

	void setLightColor(const glm::vec3 & ambient, const glm::vec3 & diffuse,
						const glm::vec3 & specular, float shininess = 1.0)
	{
		_ambient = ambient;
		_diffuse = diffuse;
		_specular = specular;
		_shininess = shininess;
		makeDirty();
	}

	void setLightColor(float ambientR, float ambientG, float ambientB,
						float diffuseR, float diffuseG, float diffuseB,
						float specularR, float specularG, float specularB,
						float shininess = 1.0)
	{
		_ambient = glm::vec3(ambientR, ambientG, ambientB);
		_diffuse = glm::vec3(diffuseR, diffuseG, diffuseB);
		_specular = glm::vec3(specularR, specularG, specularB);
		_shininess = shininess;
		makeDirty();
	}

	void setDirection(const glm::vec3 & direction) { _direction = direction; makeDirty(); }

	bool getLightOn() const { return _lightOn == true; }
	void turnOn() { _lightOn = true; makeDirty(); }
	void turnOff() { _lightOn = false; makeDirty(); }


	void increaseShaderRenfence() {
		_shaderRenfence++;
		_dirty = max(_dirty, _shaderRenfence);
	}
	void decreaseDirty() { _dirty--; }
	bool dirty() const { return _dirty != 0; }

private:

	void makeDirty() { _dirty = _shaderRenfence; }

private:

	int _dirty = 0;
	int _shaderRenfence = 0; // referenced by N shader

	int _type = DIRECTION_LIGHT;

	bool _lightOn = true;

	// for all kind of lights
	glm::vec3 _ambient = glm::vec3(0.1f);
	glm::vec3 _diffuse = glm::vec3(0.5f);
	glm::vec3 _specular = glm::vec3(0.3f);
	float _shininess = 1.0f;

	// only for direction light
	glm::vec3 _direction = glm::vec3(0.0f);
};

typedef shared_ptr<Light> LightPtr;

#endif