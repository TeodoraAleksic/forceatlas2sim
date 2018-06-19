#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <GLFW\glfw3native.h>

#include <iostream>
#include <string>

#include "camera.h"
#include "graphobject.h"
#include "graphnode.h"
#include "fileparser.h"
#include "nbody.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HIGHT = 600;

double deltaTime = 0.0f;
double lastFrame = 0.0f;

Camera camera{ glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) };

NBody nbody;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.move(MoveDirection::FWD, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.move(MoveDirection::BWD, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.move(MoveDirection::LEFT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.move(MoveDirection::RIGHT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		nbody.run();
}

void mouseCallback(GLFWwindow* window, double posX, double posY)
{
	camera.turn(posX, posY, deltaTime);
}

void scrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
	camera.zoom(offsetX, offsetY);
}

int main(int argc, char** argv)
{
	std::string help = "Usage: forceatlas2sim.exe (-i INPUT) [-h]\n\n"\
		"GPU-based parallelization and graphical simulation of the ForceAtlas2 algorithm\n\n"\
		"Options:\n"\
		"-i INPUT   Input GEFX or GML graph file. Required.\n"\
		"-h         Prints usage.\n";

	std::string inputFile;

	switch (argc)
	{
	case 1:
		std::cout << help;
		return 0;
	case 2:
		if (argv[1] == std::string("-h"))
		{
			std::cout << help;
			return 0;
		}
	case 3:
		if (argv[1] == std::string("-i"))
		{
			inputFile = argv[2];
			break;
		}
	default:
		std::cout << "Invalid arguments. ";
		std::cout << help;
		return 1;
	}

	FileParser fileParser;
	GraphObject graphObject = fileParser.parse(inputFile);

	// Sets up OpenGL
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HIGHT, "ForceAtlas2Sim", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Addresses OS specific OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	glViewport(0, 0, 800, 600);

	// Sets callbacks for the GLFW window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	GraphNode graphNode(camera, graphObject);
	graphNode.init();

	nbody.init();
	nbody.setArguments(graphNode);

	// Rendering loop
	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		graphNode.draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
