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

	// void calculateOrientation()
	// {
	// 	glm::vec3 o = glm::vec3(-1.0f, 0.0f, -1.0f);
	// 	_orientation = glm::normalize(o);
	// }
	
	// keyboard function
	// void onKeyBoard(int key);

	void update(const Terrain & terrain);

	// void update(const Terrain & terrain) {
	// 	glm::vec3 position = getPosition();
	// 	float height = terrain.getHeight(position.x, position.z);
	// 	setPosition(position.x, height, position.z);
	// }

private:

	// glm::vec3 _orientation;
	float _angleWithX = 270.0f;

	float _currentSpeed = 0;
	float _turnSpeed = 0;

};

#endif