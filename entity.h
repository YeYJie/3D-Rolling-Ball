#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "include.h"
#include "model.h"

class Entity
{

public:

	Entity() = default;

	Entity(TexturedModelPtr model, glm::vec3 position,
			glm::vec3 rotation, float scale)
		: _model(model), _position(position), _rotation(rotation),
			_scale(scale) {}

	Entity(TexturedModelPtr model, glm::vec3 position,
			float rotX, float rotY, float rotZ, float scale)
		: _model(model), _position(position),
			_rotation(glm::vec3(rotX, rotY, rotZ)), _scale(scale) {}

	Entity(TexturedModelPtr model, GLuint textureIndex, glm::vec3 position,
			glm::vec3 rotation, float scale)
		: _model(model), _position(position), _rotation(rotation),
			_scale(scale), _textureIndex(textureIndex) {}

	Entity(TexturedModelPtr model, GLuint textureIndex, glm::vec3 position,
			float rotX, float rotY, float rotZ, float scale)
		: _model(model), _position(position), _textureIndex(textureIndex),
			_rotation(glm::vec3(rotX, rotY, rotZ)), _scale(scale) {}


	TexturedModelPtr getModel() const { return _model; }
	void setModel(TexturedModelPtr model) { _model = model; }

	glm::vec3 getPosition() const { return _position; }
	void setPosition(const glm::vec3 & position) { _position = position; }
	void setPosition(float x, float y, float z) {
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	float getPositionX() const { return _position.x; }
	float getPositionY() const { return _position.y; }
	float getPositionZ() const { return _position.z; }

	float & getPositionXinRefence() { return _position.x; }
	float & getPositionYinRefence() { return _position.y; }
	float & getPositionZinRefence() { return _position.z; }

	void setPositionX(float x) { _position.x = x; }
	void setPositionY(float y) { _position.y = y; }
	void setPositionZ(float z) { _position.z = z; }

	glm::vec3 getRotation() const { return _rotation; }
	void setRotation(const glm::vec3 & rotation) { _rotation = rotation; }
	void setRotation(float x, float y, float z) {
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
	}

	float getScale() const { return _scale; }
	void setScale(float scale) { _scale = scale; }

	void move(const glm::vec3 & delta) {
		_position += delta;
	}
	void move(float dx, float dy, float dz) {
		_position.x += dx;
		_position.y += dy;
		_position.z += dz;
	}

	void rotate(const glm::vec3 & delta) {
		_rotation += delta;
	}
	void rotate(float dx, float dy, float dz) {
		_rotation.x += dx;
		_rotation.y += dy;
		_rotation.z += dz;
	}

	virtual glm::mat4 getModelMatrix() const {
		glm::mat4 res = glm::mat4(1.0f);
		res = glm::translate(res, _position);
		res = glm::scale(res, glm::vec3(_scale));

		static glm::vec3 __xaxis = glm::vec3(1.0, 0.0, 0.0);
		static glm::vec3 __yaxis = glm::vec3(0.0, 1.0, 0.0);
		static glm::vec3 __zaxis = glm::vec3(0.0, 0.0, 1.0);

		res = glm::rotate(res, _rotation.x, __xaxis);
		res = glm::rotate(res, _rotation.y, __yaxis);
		res = glm::rotate(res, _rotation.z, __zaxis);

		return res;
	}

private:

	TexturedModelPtr _model;

	glm::vec3 _position = glm::vec3(0.0f);
	glm::vec3 _rotation = glm::vec3(0.0f);
	float _scale = 1.0f;

	GLuint _textureIndex = 0;

};

typedef shared_ptr<Entity> EntityPtr;

#endif