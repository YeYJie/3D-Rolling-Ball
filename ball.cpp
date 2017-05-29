#include "ball.h"

extern int keyPressed;
extern int mouseX;
extern int mouseY;
extern bool mouseLeftPressed;
extern bool mouseRightPressed;
extern int mouseScrollOffset;

void Ball::update(const TerrainPtr & terrain)
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

	_accelerationTerrain = terrain->getGradient(getPositionX(), getPositionZ());
	_accelerationTerrain *= 0.1f;

	_acceleration = _accelerationTerrain + _accelerationKeyboard + friction;
	// _acceleration = _accelerationKeyboard + friction;

	// cout << _speed.x << " " << _speed.y << " " <<  _speed.z << " "
	// 	<< friction.x << " " << friction.y << " " << friction.z << endl;

	static float t = 0.1f;

	_speed = _speed + t * _acceleration;

	if(glm::length(_speed) <= 0.01)
		_speed = glm::vec3(0.0f);

	move(_speed * t);

	setPositionY(4 + terrain->getHeight(getPositionX(), getPositionZ()));

	terrain->correctPosition(getPositionXinRefence(), getPositionZinRefence());
}

void printVec(const glm::vec3 & v, const char * s = "vec3")
{
	printf("%s : %f %f %f\n", s, v.x, v.y, v.z);
}

void printMatrix(const glm::mat4 & m, const char * s = "matrix")
{
	printf("%s :\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n",
		s,
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]);
}

glm::mat4 Ball::getModelMatrix() const
{
	static glm::vec3 __xaxis = glm::vec3(1.0, 0.0, 0.0);
	static glm::vec3 __yaxis = glm::vec3(0.0, 1.0, 0.0);
	static glm::vec3 __zaxis = glm::vec3(0.0, 0.0, 1.0);

	static glm::mat4 res = glm::mat4(1.0f);

	static float xDegree = 0.0f;
	static float zDegree = 0.0f;

	if(_speed != glm::vec3(0.0f))
	{
		res[3][0] = 0.0f;
		res[3][1] = 0.0f;
		res[3][2] = 0.0f;

		glm::vec3 rotate_axis = glm::cross(_speed, glm::vec3(0.0f, 1.0f, 0.0f));
		if(rotate_axis != glm::vec3(0.0f) 
			&& !isnan(rotate_axis.x) 
			&& !isnan(rotate_axis.y) 
			&& !isnan(rotate_axis.z))
		{
			res = glm::rotate(glm::mat4(1.0f), -0.01f * glm::length(_speed), rotate_axis) * res;
		}
	}

	res[3][0] = getPositionX();
	res[3][1] = getPositionY();
	res[3][2] = getPositionZ();
	return res;
}