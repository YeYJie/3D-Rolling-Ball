#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "include.h"
#include "ball.h"
#include "terrain.h"

class Camera
{

public:

	Camera(const Ball * ball);

	void update(const Terrain & terrain);

	glm::mat4 getViewMatrix() const {
		return glm::lookAt(_position, _ball->getPosition(),
								glm::vec3(0.0f, 1.0f, 0.0f));
	}

private:

	const Ball * _ball;

	glm::vec3 _position;

	float _pitch = 10.0f;
	float _distanceFromBall = 30.0f;
	float _angleWithX = 270.0f;

};

#endif