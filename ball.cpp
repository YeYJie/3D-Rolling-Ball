#include "ball.h"

extern int keyPressed;
extern int mouseX;
extern int mouseY;
extern bool mouseLeftPressed;
extern bool mouseRightPressed;
extern int mouseScrollOffset;

void Ball::update(const Terrain & terrain)
{
	// mouse
	static int _lastX;
	if(mouseLeftPressed) {
		float deltax = (_lastX - mouseX) * 0.1;
		_angleWithX += deltax;
	}
	_lastX = mouseX;

	// keyboard
	glm::vec3 orientation = getOrientation();

	static const float speed = 1.0f;
	if(keyPressed == GLFW_KEY_UP || keyPressed == GLFW_KEY_W) {
		// move(orientation.x * speed, 0, orientation.z * speed);
		_accelerationKeyboard = orientation;
	}
	else if(keyPressed == GLFW_KEY_DOWN || keyPressed == GLFW_KEY_S) {
		// move(-orientation.x * speed, 0, -orientation.z * speed);
		_accelerationKeyboard = -orientation;
	}
	else if(keyPressed == GLFW_KEY_LEFT || keyPressed == GLFW_KEY_A) {
		// orientation = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), orientation);
		// move(orientation.x * speed, 0, orientation.z * speed);
		_accelerationKeyboard = glm::cross(
											glm::vec3(0.0f, 1.0f, 0.0f),
											orientation
										);
	}
	else if(keyPressed == GLFW_KEY_RIGHT || keyPressed == GLFW_KEY_D) {
		// orientation = glm::cross(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		// move(orientation.x * speed, 0, orientation.z * speed);
		_accelerationKeyboard = glm::cross(
											orientation,
											glm::vec3(0.0f, 1.0f, 0.0f)
										);
	}
	else {
		_accelerationKeyboard = glm::vec3(0.0f);
	}

	static float frictionFactor = -0.05;
	glm::vec3 friction;
	if(_speed != glm::vec3(0.0f))
		friction = frictionFactor * _speed;
	else
		friction = glm::vec3(0.0f);

	_accelerationTerrain = terrain.getGradient(getPositionX(), getPositionZ());
	_accelerationTerrain *= 0.1f;

	// _acceleration = _accelerationTerrain + _accelerationKeyboard + friction;
	_acceleration = _accelerationKeyboard + friction;

	// cout << _speed.x << " " << _speed.y << " " <<  _speed.z << " "
	// 	<< friction.x << " " << friction.y << " " << friction.z << endl;

	static float t = 0.1f;

	_speed = _speed + t * _acceleration;

	move(_speed * t);

	setPositionY(5 + terrain.getHeight(getPositionX(), getPositionZ()));

	// cout << "ball height : " << getPositionY() << " " <<  getPositionX() << " " << getPositionZ() << endl; 
}