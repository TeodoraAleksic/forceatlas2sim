#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

// Enum representing the directions the camera can move in
enum MoveDirection { FWD, BWD, LEFT, RIGHT };

/** 
* Simulation camera
*
* Handles the camera's movement in the simulation.
* Calculates matrices used for drawing the graph.
*/
class Camera
{
private:

	// Width of the screen
	const int screenWidth;
	// Height of the screen
	const int screenHeight;

	// Vector representing the camera's position in 3D space
	glm::vec3 cameraPos;
	// Vector representing the direction the camera is pointing to
	glm::vec3 cameraFront;
	// Vector perpendicular to the position and direction vectors
	glm::vec3 cameraUp;

	// The previous x axis position
	double lastX;
	// The previous y axis position
	double lastY;

	// The pitch of the camera
	double cameraPitch;
	// The yaw of the camera
	double cameraYaw;

	// Field of View
	double fov;

	// The distance of the near plane from the camera
	double nearPlain;
	// The distance of the far plane from the camera
	double farPlain;

	// Updates the direction the camera is pointing to
	void updateFront();

public:

	Camera(int screenWidth_, int screenHeight_, glm::vec3 cameraPos_, glm::vec3 cameraFront_, glm::vec3 cameraUp_);
	~Camera();

	// Gets the vector representing the camera's position in 3D space
	glm::vec3 getCameraPos() const;

	// Gets a view matrix based on the camera's current position
	glm::mat4 getViewMatrix() const;
	// Gets a projection matrix
	glm::mat4 getProjectionMatrix() const;

	/**
	* Changes the camera's position in 3D space
	*
	* @param direction The direction the camera is moving towards
	* @param deltaTime The delta time it took to execute the last frame
	*/
	void move(MoveDirection direction, double deltaTime);

	/**
	* Change's the direction the camera is pointing to
	*
	* @param posX		The new x axis position
	* @param posY		The new y axis position
	* @param deltaTime	The delta time it took to execute the last frame
	* @param reset		Re-initializes the camera's direction
	*/
	void turn(double posX, double posY, double deltaTime, bool reset);

	/**
	* Zooms the camera in
	*
	* @param offsetX The scroll offset along the x axis
	* @param offsetY The scroll offset along the y axis
	*/
	void zoom(double offsetX, double offsetY);

};

#endif
