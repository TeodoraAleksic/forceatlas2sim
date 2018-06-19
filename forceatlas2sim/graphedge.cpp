#include "graphedge.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

GraphEdge::GraphEdge(Camera& camera_, GraphObject& graphObject_) : camera(camera_), graphObject(graphObject_)
{
	isInited = false;
	vao = 0;
	vboVertex = 0;
	vboIndex = 0;
	vboSourceX = 0;
	vboSourceY = 0;
	vboSourceZ = 0;
	vboTargetX = 0;
	vboTargetY = 0;
	vboTargetZ = 0;
	program = 0;
}

GraphEdge::~GraphEdge()
{
	cleanup();
}

void GraphEdge::init()
{
	if (isInited)
		return;

	// Builds shaders
	std::vector<unsigned int> shaders;
	shaders.push_back(buildShader("edge.vert", GL_VERTEX_SHADER));
	shaders.push_back(buildShader("edge.frag", GL_FRAGMENT_SHADER));

	// Builds program
	program = buildProgram(shaders);

	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		glDeleteShader(*iter);

	// Gets uniform variable locations
	uniformProjection = glGetUniformLocation(program, "projection");
	uniformView = glGetUniformLocation(program, "view");
	uniformModel = glGetUniformLocation(program, "model");

	float vertices[] = {
		1.0f, 0.025f, 0.025f,
		1.0f, -0.025f, 0.025f,
		0.0f, -0.025f, 0.025f,
		0.0f, 0.025f, 0.025f,
		0.0f, -0.025f, -0.025f,
		0.0f, 0.025f, -0.025f,
		1.0f, 0.025f, -0.025f,
		1.0f, -0.025f, -0.025f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,
		2, 3, 5,
		2, 4, 5,
		4, 5, 6,
		4, 6, 7,
		0, 6, 7,
		1, 6, 7,
		1, 2, 4,
		1, 4, 7,
		0, 3, 5,
		0, 5, 6
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Binds vertices
	glGenBuffers(1, &vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Binds indices
	glGenBuffers(1, &vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	// TODO
}

void GraphEdge::draw()
{
	if (!isInited)
		return;

	glUseProgram(program);

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HIGHT = 600;

	glm::mat4 projection = glm::perspective((float)glm::radians(camera.getFOV()), (float)(SCREEN_WIDTH / SCREEN_HIGHT), 0.1f, 1000.0f);
	glm::mat4 view = camera.getPosition();
	glm::mat4 model(1.0f);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);

	//  TODO glDrawElementsInstanced(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_INT, NULL, graphObject.getNumOfEdges());

	glFinish();
}

void GraphEdge::cleanup()
{
	if (!isInited)
		return;

	if (vboTargetX)
		glDeleteBuffers(1, &vboTargetX);

	if (vboTargetY)
		glDeleteBuffers(1, &vboTargetY);

	if (vboTargetZ)
		glDeleteBuffers(1, &vboTargetZ);

	if (vboSourceX)
		glDeleteBuffers(1, &vboSourceX);

	if (vboSourceY)
		glDeleteBuffers(1, &vboSourceY);

	if (vboSourceZ)
		glDeleteBuffers(1, &vboSourceZ);

	if (vboIndex)
		glDeleteBuffers(1, &vboIndex);

	if (vboVertex)
		glDeleteBuffers(1, &vboVertex);

	if (vao)
		glDeleteVertexArrays(1, &vao);

	if (program)
		glDeleteProgram(program);

	isInited = false;
	vao = 0;
	vboVertex = 0;
	vboIndex = 0;
	vboSourceX = 0;
	vboSourceY = 0;
	vboSourceZ = 0;
	vboTargetX = 0;
	vboTargetY = 0;
	vboTargetZ = 0;
	program = 0;
}

unsigned int GraphEdge::getNumOfEdges() const
{
	return 0; // TODO graphObject.getNumOfEdges()
}

unsigned int GraphEdge::getSourceX() const
{
	return vboSourceX;
}

unsigned int GraphEdge::getSourceY() const
{
	return vboSourceY;
}

unsigned int GraphEdge::getSourceZ() const
{
	return vboSourceZ;
}

unsigned int GraphEdge::getTargetX() const
{
	return vboTargetX;
}

unsigned int GraphEdge::getTargetY() const
{
	return vboTargetY;
}

unsigned int GraphEdge::getTargetZ() const
{
	return vboTargetZ;
}