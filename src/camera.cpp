#include "camera.h"

extern const float WATERHEIGHT;

extern int keyPressed;
extern int mouseX;
extern int mouseY;
extern bool mouseLeftPressed;
extern bool mouseRightPressed;
extern int mouseScrollOffset;

Camera::Camera(EntityPtr ball)
	// : _ball(dynamic_pointer_cast<BallPtr>(ball))
{
	_ball = dynamic_pointer_cast<Ball>(ball);

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

void Camera::update(const TerrainPtr & terrain)
{
	_distanceFromBall -= 10 * mouseScrollOffset;
	if(_distanceFromBall < 10)
		_distanceFromBall = 10;
	// else if(_distanceFromBall > 100)
	// 	_distanceFromBall = 100;

	static int _lastY;
	if(mouseRightPressed) {
		float deltay = (_lastY - mouseY) * 0.1;
		_pitch += deltay;
		if(_pitch < -80) _pitch = -80;
		else if(_pitch > 80) _pitch = 80;
	}
	_lastY = mouseY;

	// glm::vec3 o = _ball->getOrientation();
	float angleWithX = _ball->getAngleWithX();

	// cout << angleWithX << endl;

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
		3.0f + terrain->getHeight(_position.x, _position.z));
	// _position.y = max(_position.y, WATERHEIGHT + 2);
	terrain->correctPosition(_position.x, _position.z);
	// cout << "camera : " << _position.y << " " << angleWithX << endl;
}