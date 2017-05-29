#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "include.h"
#include "entity.h"
#include "ball.h"
#include "terrain.h"

class Camera
{

public:

	Camera(EntityPtr ball);

	void update(const TerrainPtr & terrain);

	glm::vec3 getPosition() const { return _position; }

	void setPosition(const glm::vec3 & position) { _position = position; }
	void setPosition(const float & x, const float & y, const float & z) {
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	glm::mat4 getViewMatrix() const {
		return glm::lookAt(_position, _ball->getPosition(),
								glm::vec3(0.0f, 1.0f, 0.0f));
	}

private:

	BallPtr _ball;

	glm::vec3 _position;

	float _pitch = 45.0f;
	float _distanceFromBall = 20.0f;
	// float _angleWithX = 1.0f;

};

typedef shared_ptr<Camera> CameraPtr;

#endif