#include "camera.h"

extern int keyPressed;
extern int mouseX;
extern int mouseY;
extern bool mouseLeftPressed;
extern bool mouseRightPressed;
extern int mouseScrollOffset;

Camera::Camera(const Ball * ball)
{
	_ball = ball;

	float angleWithX = _ball->getAngleWithX();
	glm::vec3 p = _ball->getPosition();
	// float theta = atan(o.z / o.x);
	// float m = _distanceFromBall * cos(_pitch * 180.0 / PI);
	float m = _distanceFromBall * cos(_pitch * 0.0174533f);

	// _position.x = p.x + m * cos(theta);
	_position.x = p.x + m * cos(angleWithX * 0.0174533f);

	_position.y = p.y + _distanceFromBall * sin(_pitch * 0.0174533f);

	// _position.z = p.z - m * sin(theta);
	_position.z = p.z - m * sin(angleWithX * 0.0174533f);

	// cout << "camera : " << _pitch << " " << _distanceFromBall << " " << m << endl;
	// _position.y = max(_position.y, terrain.getHeight(_position.x, _position.z));
}

void Camera::update(const Terrain & terrain)
{
	_distanceFromBall -= 2 * mouseScrollOffset;
	if(_distanceFromBall < 10) 
		_distanceFromBall = 10;
	else if(_distanceFromBall > 100) 
		_distanceFromBall = 100;

	static int _lastY;
	if(mouseRightPressed) {
		float deltay = (_lastY - mouseY) * 0.1;
		_pitch += deltay;
		if(_pitch < 0) _pitch = 0;
		else if(_pitch > 80) _pitch = 80;
	}
	_lastY = mouseY;

	// static int _lastX;
	// if(mouseLeftPressed) {
	// 	float deltax = (_lastX - mouseX) * 0.1;
	// 	_angleWithX += deltax;		
	// }
	// _lastX = mouseX;


	// glm::vec3 o = _ball->getOrientation();
	float angleWithX = _ball->getAngleWithX();
	glm::vec3 p = _ball->getPosition();
	// float theta = atan(o.z / o.x);
	// float m = _distanceFromBall * cos(_pitch * 180.0 / PI);
	float m = _distanceFromBall * cos(_pitch * 0.0174533f);

	// _position.x = p.x + m * cos(theta);
	_position.x = p.x + m * cos(angleWithX * 0.0174533f);

	_position.y = p.y + _distanceFromBall * sin(_pitch * 0.0174533f);

	// _position.z = p.z - m * sin(theta);
	_position.z = p.z - m * sin(angleWithX * 0.0174533f);

	// cout << "camera : " << _pitch << " " << _distanceFromBall << " " << m << endl;
	_position.y = max(_position.y, 
		3.0f + terrain.getHeight(_position.x, _position.z));
	// if(_position.y < 10)
	// 	_position.y = 10;
}