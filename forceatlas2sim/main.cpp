#include <iostream>
#include <math.h>
#include <memory>
#include <string>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "camera.h"
#include "fileparser.h"
#include "forceatlas2sim.h"
#include "forceatlas2params.h"
#include "glgraphedge.h"
#include "glgraphnode.h"
#include "graphobject.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HIGHT = 600;

double deltaTime = 0.0f;
double lastFrame = 0.0f;

std::unique_ptr<Camera> camera;

std::unique_ptr<ForceAtlas2Sim> fa2Sim;

bool isValueArg(std::string argName)
{
	return 
		argName == std::string("-i")	||
		argName == std::string("-kr")	||
		argName == std::string("-krp")	||
		argName == std::string("-kg");
}

bool isFlagArg(std::string argName)
{
	return 
		argName == std::string("-fg")	|| 
		argName == std::string("-fsg")	||
		argName == std::string("-nw");
}

bool isNotArg(std::string argName)
{
	return !isValueArg(argName) && !isFlagArg(argName) && argName != std::string("-h");
}

void setValueArg(ForceAtlas2Params* fa2Params, std::string argName, std::string argValue)
{
	if (argName == std::string("-i"))
		fa2Params->setInput(argValue);
	else if (argName == std::string("-kr"))
		fa2Params->setKr(argValue);
	else if (argName == std::string("-krp"))
		fa2Params->setKrp(argValue);
	else if (argName == std::string("-kg"))
		fa2Params->setKg(argValue);
	else
		throw "Invalid value argument " + argName;
}

void setFlagArg(ForceAtlas2Params* fa2Params, std::string argName)
{
	if (argName == std::string("-fg"))
		fa2Params->setFg(true);
	else if (argName == std::string("-fsg"))
		fa2Params->setFsg(true);
	else if (argName == std::string("-nw"))
		fa2Params->setNw(true);
	else
		throw "Invalid flag argument " + argName;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->move(MoveDirection::FWD, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->move(MoveDirection::BWD, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->move(MoveDirection::LEFT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->move(MoveDirection::RIGHT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		fa2Sim->run();
}

void mouseCallback(GLFWwindow* window, double posX, double posY)
{
	camera->turn(posX, posY, deltaTime);
}

void scrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
	camera->zoom(offsetX, offsetY);
}

int main(int argc, char** argv)
{
	std::string usage = \
		"Usage: forceatlas2sim.exe (-i INPUT) [-kr FLOAT] [-krp FLOAT]\n"\
		"                          [-fg] [-fsg] [-kg FLOAT] [-nw] [-h]\n\n"\
		"GPU-based parallelization and graphical simulation of the ForceAtlas2 algorithm.\n\n"\
		"Options:\n"\
		"-i INPUT     Input GEFX or GML graph file. Required.\n"\
		"-kr FLOAT    Repulsion force coefficient. Default: 0.01.\n"\
		"-krp FLOAT   Repulsion force overlap coefficient. Default: 100.\n"\
		"-fg          Uses gravitational force to attract nodes to graph center.\n"\
		"-fsg         Uses strong gravitational force to attract nodes to graph center.\n"\
		"-kg FLOAT    Gravitational force coefficient. Default: 9.81.\n"\
		"-nw          Discards edge weights.\n"\
		"-h           Prints usage.\n";

	ForceAtlas2Params fa2Params;

	switch (argc)
	{
	case 1:
		std::cout << usage;
		return 0;
	case 2:
		if (argv[1] == std::string("-h"))
		{
			std::cout << usage;
			return 0;
		}
		else
		{
			std::cout << "Must provide input file.\n\n" << usage;
			return 1;
		}
	default:
		// Parses script arguments
		for (int i = 1; i < argc; ++i)
		{
			if (isValueArg(argv[i]))
			{
				// Looks for value after value argument
				if ((i + 1) < argc && isNotArg(argv[i + 1]))
				{
					setValueArg(&fa2Params, argv[i], argv[i + 1]);
					++i;
				}
				else
				{
					std::cout << "Must provide value after '" << argv[i] << "' argument.\n\n" << usage;
					return 1;
				}
			}
			else if (isFlagArg(argv[i]))
			{
				setFlagArg(&fa2Params, argv[i]);
			}
			else if (argv[i] == std::string("-h"))
			{
				std::cout << "Can't specify '-h' with other arguments.\n\n" << usage;
				return 1;
			}
			else 
			{
				std::cout << "Invalid argument: '" << argv[i] << "'.\n\n" << usage;
				return 1;
			}
		}

		if (fa2Params.getInput().empty())
		{
			std::cout << "Must provide input file.\n\n" << usage;
			return 1;
		}
	}

	FileParser fileParser;
	GraphObject graphObject = fileParser.parse(fa2Params.getInput());

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

	glEnable(GL_DEPTH_TEST);

	// Sets callbacks for the GLFW window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// Gets info about selected OpenGL device
	std::string glVendor = (const char*)glGetString(GL_VENDOR);
	std::string glRenderer = (const char*)glGetString(GL_RENDERER);

	std::cout << "OpenGL Device" << std::endl;
	std::cout << "VENDOR: " << glVendor << std::endl;
	std::cout << "RENDERER: " << glRenderer << std::endl;
	std::cout << std::endl;

	// Initializes camera
	camera = std::make_unique<Camera>(
		glm::vec3(0.0f, 0.0f, graphObject.getInitPosition()), 
		glm::vec3(0.0f, 0.0f, -1.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f));

	// Initializes graph node
	GLGraphNode graphNode(*camera, graphObject);
	graphNode.init();

	// Initializes graph edge
	GLGraphEdge graphEdge(*camera, graphObject);
	graphEdge.init();

	// Initializes ForceAtlas2 simulation
	fa2Sim = std::make_unique<ForceAtlas2Sim>(fa2Params, graphObject, graphNode, graphEdge);
	fa2Sim->init();

	// Runs rendering loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculates time since last loop
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draws graph
		graphEdge.draw();
		graphNode.draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
