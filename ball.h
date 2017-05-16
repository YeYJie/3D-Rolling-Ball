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


	Ball(TexturedModel model, glm::vec3 position,
			glm::vec3 rotation, float scale)
		: Entity(model, position, rotation, scale) {}

	Ball(TexturedModel model, glm::vec3 position,
			float rotX, float rotY, float rotZ, float scale)
		: Entity(model, position, rotX, rotY, rotZ, scale) {}

	Ball(TexturedModel model, GLuint textureIndex, glm::vec3 position,
			glm::vec3 rotation, float scale)
		: Entity(model, textureIndex, position, rotation, scale) {}

	Ball(TexturedModel model, GLuint textureIndex, glm::vec3 position,
			float rotX, float rotY, float rotZ, float scale)
		: Entity(model, textureIndex, position, rotX, rotY, rotZ, scale) {}

	// keyboard function
	void onKeyBoard(int key) {

		static const float forwardSpeed = 50;
		static const float turnSpeed = 50;
		
		switch(key) {
			case GLFW_KEY_UP:
			case GLFW_KEY_W:
				_currentSpeed = forwardSpeed; break;
			case GLFW_KEY_DOWN:
			case GLFW_KEY_S:
				_currentSpeed = -forwardSpeed; break;
			case GLFW_KEY_LEFT:
			case GLFW_KEY_A:
				_turnSpeed = turnSpeed; break;
			case GLFW_KEY_RIGHT:
			case GLFW_KEY_D:
				_turnSpeed = -turnSpeed; break;
			
			default:
				_currentSpeed = _turnSpeed = 0;
		}
	}

	void update(const Terrain & terrain) {
		glm::vec3 position = getPosition();
		float height = terrain.getHeight(position.x, position.z);
		setPosition(position.x, height, position.z);
	}

private:

	float _currentSpeed = 0;
	float _turnSpeed = 0;

};

#endif