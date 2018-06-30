#include <iostream>
#include <string>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "camera.h"
#include "clcontext.h"
#include "clnbody.h"
#include "clupdateedge.h"
#include "clupdatenode.h"
#include "fileparser.h"
#include "glgraphedge.h"
#include "glgraphnode.h"
#include "graphobject.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HIGHT = 600;

double deltaTime = 0.0f;
double lastFrame = 0.0f;

Camera camera{ glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) };

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
	/*else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		nbody.run();*/
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

	GLGraphNode graphNode(camera, graphObject);
	graphNode.init();

	GLGraphEdge graphEdge(camera, graphObject);
	graphEdge.init();

	CLContext clContext;

	CLNbody nbody(clContext.getDevice(), clContext.getContext());
	CLUpdateNode updateNode(clContext.getDevice(), clContext.getContext());
	CLUpdateEdge updateEdge(clContext.getDevice(), clContext.getContext());

	nbody.init();
	updateNode.init();
	updateEdge.init();

	cl::Buffer fx = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	cl::Buffer fy = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	cl::Buffer fz = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

	nbody.setWorkSize(graphObject.getNumOfNodes());
	nbody.setArg(0, graphObject.getNumOfNodes());
	nbody.setArg(1, graphNode.getOffsetX(), CL_MEM_READ_ONLY);
	nbody.setArg(2, graphNode.getOffsetY(), CL_MEM_READ_ONLY);
	nbody.setArg(3, graphNode.getOffsetZ(), CL_MEM_READ_ONLY);
	nbody.setArg(4, graphNode.getScale(), CL_MEM_READ_ONLY);
	nbody.setArg(5, fx);
	nbody.setArg(6, fy);
	nbody.setArg(7, fz);

	updateNode.setWorkSize(graphObject.getNumOfNodes());
	updateNode.setArg(0, graphObject.getNumOfNodes());
	updateNode.setArg(1, graphNode.getOffsetX(), CL_MEM_READ_WRITE);
	updateNode.setArg(2, graphNode.getOffsetY(), CL_MEM_READ_WRITE);
	updateNode.setArg(3, graphNode.getOffsetZ(), CL_MEM_READ_WRITE);   
	updateNode.setArg(4, graphNode.getScale(), CL_MEM_READ_ONLY);
	updateNode.setArg(5, fx);
	updateNode.setArg(6, fy);
	updateNode.setArg(7, fz);

	updateEdge.setWorkSize(graphObject.getNumOfEdges());
	updateEdge.setArg(0, graphObject.getNumOfEdges());
	updateEdge.setArg(1, graphNode.getOffsetX(), CL_MEM_READ_ONLY);
	updateEdge.setArg(2, graphNode.getOffsetY(), CL_MEM_READ_ONLY);
	updateEdge.setArg(3, graphNode.getOffsetZ(), CL_MEM_READ_ONLY);
	updateEdge.setArg(4, sizeof(cl_uint) * graphObject.getNumOfEdges(), &(graphObject.getSourceId())[0], CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);
	updateEdge.setArg(5, graphEdge.getSourceX(), CL_MEM_READ_WRITE);
	updateEdge.setArg(6, graphEdge.getSourceY(), CL_MEM_READ_WRITE);
	updateEdge.setArg(7, graphEdge.getSourceZ(), CL_MEM_READ_WRITE);
	updateEdge.setArg(8, sizeof(cl_uint) * graphObject.getNumOfEdges(), &(graphObject.getTargetId())[0], CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);
	updateEdge.setArg(9, graphEdge.getTargetX(), CL_MEM_READ_WRITE);
	updateEdge.setArg(10, graphEdge.getTargetY(), CL_MEM_READ_WRITE);
	updateEdge.setArg(11, graphEdge.getTargetZ(), CL_MEM_READ_WRITE);

	nbody.run();
	updateNode.run();
	updateEdge.run();

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

		graphEdge.draw();
		graphNode.draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
