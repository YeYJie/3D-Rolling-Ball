#ifndef _BALL_H_
#define _BALL_H_

#include "include.h"
#include "entity.h"
#include "terrain.h"

class Ball : public Entity
{

public:

	// singleton
	Ball(const Ball &) = delete;
	Ball & operator=(const Ball &) = delete;


	Ball(TexturedModel * model, glm::vec3 position,
			glm::vec3 rotation, float scale)
		: Entity(model, position, rotation, scale) 
	{
		// calculateOrientation();
	}

	Ball(TexturedModel * model, glm::vec3 position,
			float rotX, float rotY, float rotZ, float scale)
		: Entity(model, position, rotX, rotY, rotZ, scale)
	{
		// calculateOrientation();
	}


	Ball(TexturedModel * model, GLuint textureIndex, glm::vec3 position,
			glm::vec3 rotation, float scale)
		: Entity(model, textureIndex, position, rotation, scale)
	{
		// calculateOrientation();
	}


	Ball(TexturedModel * model, GLuint textureIndex, glm::vec3 position,
			float rotX, float rotY, float rotZ, float scale)
		: Entity(model, textureIndex, position, rotX, rotY, rotZ, scale)
	{
		// calculateOrientation();
	}

	float getAngleWithX() const { return _angleWithX; }

	glm::vec3 getOrientation() const {
		return glm::vec3(
							-(float)cos(_angleWithX * 0.0174533f),
							0.0f, 
							(float)sin(_angleWithX * 0.0174533f)
						);
	}

	void update(const Terrain & terrain);

	glm::vec3 getAccelerationTerrain() const {
		return _accelerationTerrain;
	}

	glm::vec3 getAccelerationKeyboard() const {
		return _accelerationKeyboard;
	}

	glm::vec3 getAcceleration() const {
		return _acceleration;
	}

	glm::vec3 getSpeed() const {
		return _speed;
	}

	glm::mat4 getModelMatrix() const;

private:

	float _angleWithX = 270.0f;

	glm::vec3 _accelerationTerrain;

	glm::vec3 _accelerationKeyboard;

	glm::vec3 _acceleration;

	glm::vec3 _speed = glm::vec3(0.0f);

};

#endif