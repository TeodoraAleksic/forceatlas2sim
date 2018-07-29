#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

enum MoveDirection { FWD, BWD, LEFT, RIGHT };

class Camera
{
private:

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	double lastX;
	double lastY;

	double cameraPitch;
	double cameraYaw;

	double fov;

	void update();

public:

	Camera(glm::vec3 cameraPos_, glm::vec3 cameraFront_, glm::vec3 cameraUp_);
	~Camera();

	glm::vec3 getCameraPos() const;

	glm::mat4 getPosition() const;
	double getFOV() const;

	void move(MoveDirection direction, double deltaTime);
	void turn(double posX, double posY, double deltaTime);
	void zoom(double offsetX, double offsetY);

};

#endif
