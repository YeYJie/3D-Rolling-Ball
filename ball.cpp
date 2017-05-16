#include "ball.h"

void Ball::onKeyBoard(int key)
{
	switch(key) {
		// up
		case GLFW_KEY_UP:
		case GLFW_KEY_W:
			_currentSpeed = forwardSpeed; break;
		// down
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S:
			_currentSpeed = -forwardSpeed; break;
		// left
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A:
			_turnSpeed = turnSpeed; break;
		// right
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D:
			_turnSpeed = -turnSpeed; break;

		default:
			_currentSpeed = _turnSpeed = 0;
	}
}