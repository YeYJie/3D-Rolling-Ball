#include "camera.h"

extern int keyPressed;
extern int mouseX;
extern int mouseY;
extern bool mouseLeftPressed;
extern bool mouseRightPressed;
extern int mouseScrollOffset;

void Camera::update()
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
		if(_pitch < 5) _pitch = 5;
		else if(_pitch > 80) _pitch = 80;
	}
	_lastY = mouseY;

	static int _lastX;
	if(mouseLeftPressed) {
		float deltax = (_lastX - mouseX) * 0.1;
		_angleWithX += deltax;		
	}
	_lastX = mouseX;


	glm::vec3 p = _ball->getPosition();
	float m = _distanceFromBall * cos(_pitch * 0.0174533f);
	_position.x = p.x + m * cos(_angleWithX * 0.0174533f);
	_position.y = p.y + _distanceFromBall * sin(_pitch * 0.0174533f);
	_position.z = p.z - m * sin(_angleWithX * 0.0174533f);

	// cout << "camera : " << _pitch << " " << _distanceFromBall << " " << m << endl;
}