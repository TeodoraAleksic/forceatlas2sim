#include <iostream>
#include <math.h>
#include <memory>
#include <string>

#include <spdlog\fmt\fmt.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <spdlog\spdlog.h>

#include "camera.h"
#include "fileparser.h"
#include "forceatlas2sim.h"
#include "forceatlas2params.h"
#include "glgraphedge.h"
#include "glgraphnode.h"
#include "glselect.h"
#include "gltext.h"
#include "graphobject.h"
#include "message.h"
#include "utility.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

double deltaTime = 0.0f;
double lastFrame = 0.0f;

bool runSim = false;
int oldPKeyState = GLFW_RELEASE;

bool drawEdges = true;
int oldEKeyState = GLFW_RELEASE;

bool ctrlPressed = false;
int oldCtrlKeyState = GLFW_RELEASE;

unsigned int selectedNode = -1;
bool getSelectedNode = false;
int oldLeftMouseState = GLFW_RELEASE;

bool leftMousePressed = false;
bool resetMouseCoords = false;

GLFWcursor* arrowCursor;
GLFWcursor* handCursor;

std::unique_ptr<Camera> camera;

std::unique_ptr<ForceAtlas2Sim> fa2Sim;

void processInput(GLFWwindow* window)
{
	int newPKeyState = glfwGetKey(window, GLFW_KEY_P);
	int newEKeyState = glfwGetKey(window, GLFW_KEY_E);
	int newCtrlKeyState = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);

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
	else if (newPKeyState == GLFW_PRESS && oldPKeyState == GLFW_RELEASE)
		runSim = runSim ? false : true;
	else if (newEKeyState == GLFW_PRESS && oldEKeyState == GLFW_RELEASE)
		drawEdges = drawEdges ? false : true;
	else if (newCtrlKeyState == GLFW_PRESS && oldCtrlKeyState == GLFW_RELEASE)
		ctrlPressed = true;
	else if (newCtrlKeyState == GLFW_RELEASE && oldCtrlKeyState == GLFW_PRESS)
		ctrlPressed = false;

	oldPKeyState = newPKeyState;
	oldEKeyState = newEKeyState;
	oldCtrlKeyState = newCtrlKeyState;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		int newLeftMouseState = action;

		if (ctrlPressed && newLeftMouseState == GLFW_PRESS && oldLeftMouseState == GLFW_RELEASE)
			getSelectedNode = true;

		if (newLeftMouseState == GLFW_PRESS)
			glfwSetCursor(window, handCursor);
		else
			glfwSetCursor(window, arrowCursor);

		leftMousePressed = (newLeftMouseState == GLFW_PRESS);
		resetMouseCoords = (newLeftMouseState == GLFW_PRESS) && (oldLeftMouseState == GLFW_RELEASE);

		oldLeftMouseState = newLeftMouseState;
	}
}

void cursorCallback(GLFWwindow* window, double posX, double posY)
{
	if (leftMousePressed)
	{
		camera->turn(posX, posY, deltaTime, resetMouseCoords);
		resetMouseCoords = false;
	}
}

void scrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
	camera->zoom(offsetX, offsetY);
}

int main(int argc, char** argv)
{
	std::string usage = \
		"Usage: forceatlas2sim.exe (-i INPUT) [-kr FLOAT] [-krp FLOAT] [-fg]\n"
		"                          [-fsg] [-kg FLOAT] [-tau FLOAT] [-ks FLOAT]\n"
		"                          [-ksmax FLOAT] [-delta INT] [-h]\n\n"
		"GPU-based parallelization and graphical simulation of the ForceAtlas2 algorithm.\n\n"\
		"Options:\n"\
		"-i INPUT      Input GEFX or GML graph file. Required.\n"\
		"-kr FLOAT     Repulsion force coefficient. Default: 0.01.\n"\
		"-krp FLOAT    Repulsion force overlap coefficient. Default: 100.\n"\
		"-fg           Uses gravitational force to attract nodes to graph center.\n"\
		"-fsg          Uses strong gravitational force to attract nodes to graph center.\n"\
		"-kg FLOAT     Gravitational force coefficient. Default: 1.0.\n"\
		"-tau FLOAT    Tolerance to swinging coefficient. Default: 0.5.\n"\
		"-ks FLOAT     Global speed coefficient. Default: 0.1.\n"\
		"-ksmax FLOAT  Max global speed coefficient. Default: 10.\n"\
		"-delta INT    Edge weight influence coefficient. Default: 1.\n"\
		"-debug        Turns on debug mode.\n"\
		"-h            Prints usage.\n";

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
			if (fa2Params.isValueArg(argv[i]))
			{
				// Looks for value after value argument
				if ((i + 1) < argc && fa2Params.isNotArg(argv[i + 1]))
				{
					fa2Params.setValueArg(argv[i], argv[i + 1]);
					++i;
				}
				else
				{
					std::cout << "Must provide value after '" << argv[i] << "' argument.\n\n" << usage;
					return 1;
				}
			}
			else if (fa2Params.isFlagArg(argv[i]))
			{
				fa2Params.setFlagArg(argv[i]);
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

	if (fa2Params.getDebug()) 
		spdlog::set_level(spdlog::level::debug);

	FileParser fileParser;
	GraphObject graphObject = fileParser.parse(fa2Params.getInput());

	// Sets up OpenGL
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ForceAtlas2Sim", glfwGetPrimaryMonitor(), NULL);

	if (window == NULL)
	{
		spdlog::error(msg::ERR_GLFW_WINDOW);
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Addresses OS specific OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::error(msg::ERR_GLAD_INIT);
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Sets callbacks for the GLFW window
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// Creates cursors
	arrowCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

	// Gets info about selected OpenGL device
	std::string glVendor = (const char*)glGetString(GL_VENDOR);
	std::string glRenderer = (const char*)glGetString(GL_RENDERER);

	spdlog::info(fmt::format(msg::INFO_GL_VENDOR, glVendor));
	spdlog::info(fmt::format(msg::INFO_GL_RENDERER, glRenderer));

	// Initializes camera
	camera = std::make_unique<Camera>(
		SCREEN_WIDTH, SCREEN_HEIGHT,
		glm::vec3(0.0f, 0.0f, graphObject.getInitPosition()),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	// Initializes graph node
	GLGraphNode graphNode(*camera, graphObject);
	graphNode.init();

	// Initializes graph edge
	GLGraphEdge graphEdge(*camera, graphObject);
	graphEdge.init();

	// Initializes object for node selection
	GLSelect graphSelection(*camera, graphObject);

	graphSelection.setVboVertex(graphNode.getVboVertices());
	graphSelection.setVboIndex(graphNode.getVboIndices());
	graphSelection.setVboOffsetX(graphNode.getVboOffsetX());
	graphSelection.setVboOffsetY(graphNode.getVboOffsetY());
	graphSelection.setVboOffsetZ(graphNode.getVboOffsetZ());
	graphSelection.setVboScale(graphNode.getVboScale());

	graphSelection.init();

	// Initializes object for text drawing
	GLText graphText(*camera, graphObject);
	graphText.init();

	// Initializes ForceAtlas2 simulation
	fa2Sim = std::make_unique<ForceAtlas2Sim>(fa2Params, graphObject, graphNode, graphEdge);
	fa2Sim->init();

	spdlog::info(msg::INFO_START_SIMULATION);

	// Initializes variables for calculating the delta run time
	unsigned int numOfRunLoops = 0;
	std::vector<double> deltaRunTime;

	// Runs rendering loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculates time since last loop
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		processInput(window);

		double currentRunFrame = glfwGetTime();

		// Runs ForceAtlas2
		if (runSim) fa2Sim->run();

		// Saves time of run loop if debug mode is on
		if (runSim && fa2Params.getDebug())
		{
			++numOfRunLoops;
			deltaRunTime.push_back(glfwGetTime() - currentRunFrame);
		}

		// Gets selected node on screen
		if (getSelectedNode && !runSim)
		{
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			graphSelection.draw();

			double cursorX, cursorY;
			glfwGetCursorPos(window, &cursorX, &cursorY);

			unsigned char data[4];
			glReadPixels((GLint)cursorX, SCREEN_HEIGHT - (GLint)cursorY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

			selectedNode = data[0] + data[1] * 256 + data[2] * 265 * 265;

			graphEdge.setSelectedNode(selectedNode);
			graphNode.setSelectedNode(selectedNode);
			graphText.setSelectedNode(selectedNode);
		}

		getSelectedNode = false;

		glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draws graph
		if (drawEdges) graphEdge.draw();
		graphNode.draw();
		graphText.draw();

		glfwSwapBuffers(window);
	}

	// Calculates the delta run time of the simulation
	double deltaRunSum = 0;
	for (unsigned int i = 0; i < deltaRunTime.size(); ++i)
		deltaRunSum += deltaRunTime[i];

	if (numOfRunLoops > 0)
		spdlog::debug(fmt::format(msg::DEBUG_DELTA_RUN_TIME, deltaRunSum * 1000 / numOfRunLoops));

	glfwDestroyCursor(arrowCursor);
	glfwDestroyCursor(handCursor);

	glfwTerminate();

	return 0;
}
