#include "camera.h"

Camera::Camera(int screenWidth_, int screenHeight_, glm::vec3 cameraPos_, glm::vec3 cameraFront_, glm::vec3 cameraUp_) :
	screenWidth(screenWidth_), screenHeight(screenHeight_),
	cameraPos(cameraPos_), cameraFront(cameraFront_), cameraUp(cameraUp_), 
	cameraPitch(0.0f), cameraYaw(-90.0f), fov(45.0f),
	nearPlain(0.1), farPlain(5000)
{
}

Camera::~Camera()
{
}

void Camera::update()
{
	// Updates camera vectors
	glm::vec3 front;
	front.x = (float)(cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw)));
	front.y = (float)(sin(glm::radians(cameraPitch)));
	front.z = (float)(cos(glm::radians(cameraPitch)) * sin(glm::radians(cameraYaw)));

	cameraFront = glm::normalize(front);
}

glm::vec3 Camera::getCameraPos() const
{
	return cameraPos;
}

glm::mat4 Camera::getPosition() const
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::getPerspective() const
{
	return glm::perspective((float)glm::radians(fov), (float)(screenWidth / screenHeight), 0.1f, 5000.0f);
}

void Camera::move(MoveDirection direction, double deltaTime)
{
	float cameraSpeed = (float)(100.0f * deltaTime);

	// Moves camera in given direction
	switch (direction)
	{
	case FWD:
		cameraPos += cameraSpeed * cameraFront;
		break;
	case BWD:
		cameraPos -= cameraSpeed * cameraFront;
		break;
	case LEFT:
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case RIGHT:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	default:
		break;
	}

	update();
}

void Camera::turn(double posX, double posY, double deltaTime, bool reset)
{
	// Initializes camera position
	if (reset)
	{
		lastX = posX;
		lastY = posY;
	}

	// Calculates camera movement since last position
	double xoffset = lastX - posX;
	double yoffset = posY - lastY;
	lastX = posX;
	lastY = posY;

	// Calculates new camera offset
	double sensitivity = 10.0f * deltaTime;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cameraPitch += yoffset;
	cameraYaw += xoffset;

	// Restricts camera up/down movements
	if (cameraPitch > 89.0f) cameraPitch = 89.0f;
	if (cameraPitch < -89.0f) cameraPitch = -89.0f;

	update();
}

void Camera::zoom(double offsetX, double offsetY)
{
	// Increases/decreases field of view
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= offsetY;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
