#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glgraphedge.h"
#include "shader.h"

GLGraphEdge::GLGraphEdge(const Camera& camera_, const GraphObject& graphObject_) : camera(camera_), graphObject(graphObject_)
{
	selectedNode = -1;
	isInited = false;
	vao = 0;
	vboVertex = 0;
	vboIndex = 0;
	vboSourceId = 0;
	vboTargetId = 0;
	vboSourceX = 0;
	vboSourceY = 0;
	vboSourceZ = 0;
	vboTargetX = 0;
	vboTargetY = 0;
	vboTargetZ = 0;
	program = 0;
}

GLGraphEdge::~GLGraphEdge()
{
	cleanup();
}

void GLGraphEdge::init()
{
	if (isInited)
		return;

	// Builds shaders
	std::vector<unsigned int> shaders;
	shaders.push_back(buildShader(GL_VERTEX_SHADER, shader::edgeVert));
	shaders.push_back(buildShader(GL_FRAGMENT_SHADER, shader::edgeFrag));

	// Builds program
	program = buildProgram(shaders);

	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		glDeleteShader(*iter);

	// Gets uniform variable locations
	uniformProjection = glGetUniformLocation(program, "projection");
	uniformView = glGetUniformLocation(program, "view");
	uniformModel = glGetUniformLocation(program, "model");
	uniformSelectedNode = glGetUniformLocation(program, "selectedNode");

	// Rectangle vertices
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

	// Rectangle indices
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

	// Binds edge Ids
	std::vector<unsigned int> sourceId = graphObject.getSourceId();
	std::vector<unsigned int> targetId = graphObject.getTargetId();

	glGenBuffers(1, &vboSourceId);
	glBindBuffer(GL_ARRAY_BUFFER, vboSourceId);
	glBufferData(GL_ARRAY_BUFFER, sourceId.size() * sizeof(GLuint), &sourceId[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &vboTargetId);
	glBindBuffer(GL_ARRAY_BUFFER, vboTargetId);
	glBufferData(GL_ARRAY_BUFFER, targetId.size() * sizeof(GLuint), &targetId[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(2, 1);

	// Binds source offsets
	std::vector<float> sourceX = graphObject.getSourceX();
	std::vector<float> sourceY = graphObject.getSourceY();
	std::vector<float> sourceZ = graphObject.getSourceZ();

	glGenBuffers(1, &vboSourceX);
	glBindBuffer(GL_ARRAY_BUFFER, vboSourceX);
	glBufferData(GL_ARRAY_BUFFER, sourceX.size() * sizeof(GLfloat), &sourceX[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(3, 1);

	glGenBuffers(1, &vboSourceY);
	glBindBuffer(GL_ARRAY_BUFFER, vboSourceY);
	glBufferData(GL_ARRAY_BUFFER, sourceY.size() * sizeof(GLfloat), &sourceY[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(4, 1);

	glGenBuffers(1, &vboSourceZ);
	glBindBuffer(GL_ARRAY_BUFFER, vboSourceZ);
	glBufferData(GL_ARRAY_BUFFER, sourceZ.size() * sizeof(GLfloat), &sourceZ[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(5, 1);

	// Binds target offsets
	std::vector<float> targetX = graphObject.getTargetX();
	std::vector<float> targetY = graphObject.getTargetY();
	std::vector<float> targetZ = graphObject.getTargetZ();

	glGenBuffers(1, &vboTargetX);
	glBindBuffer(GL_ARRAY_BUFFER, vboTargetX);
	glBufferData(GL_ARRAY_BUFFER, targetX.size() * sizeof(GLfloat), &targetX[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(6, 1);

	glGenBuffers(1, &vboTargetY);
	glBindBuffer(GL_ARRAY_BUFFER, vboTargetY);
	glBufferData(GL_ARRAY_BUFFER, targetY.size() * sizeof(GLfloat), &targetY[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(7, 1);

	glGenBuffers(1, &vboTargetZ);
	glBindBuffer(GL_ARRAY_BUFFER, vboTargetZ);
	glBufferData(GL_ARRAY_BUFFER, targetZ.size() * sizeof(GLfloat), &targetZ[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(8, 1);

	isInited = true;
}

void GLGraphEdge::draw()
{
	if (!isInited)
		return;

	glUseProgram(program);

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HIGHT = 600;

	glm::mat4 projection = camera.getPerspective();
	glm::mat4 view = camera.getPosition();
	glm::mat4 model(1.0f);

	// Sets uniforms for camera position
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1ui(uniformSelectedNode, selectedNode);

	// Binds buffers
	glBindVertexArray(vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);

	// Draws edges
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_INT, NULL, graphObject.getNumOfEdges());

	glFinish();
}

void GLGraphEdge::cleanup()
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

	if (vboTargetId)
		glDeleteBuffers(1, &vboTargetId);

	if (vboSourceId)
		glDeleteBuffers(1, &vboSourceId);

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

void GLGraphEdge::setSelectedNode(unsigned int selectedNode_)
{
	selectedNode = selectedNode_;
}

unsigned int GLGraphEdge::getNumOfEdges() const
{
	return graphObject.getNumOfEdges();
}

unsigned int GLGraphEdge::getSourceId() const
{
	return vboSourceId;
}

unsigned int GLGraphEdge::getTargetId() const
{
	return vboTargetId;
}

unsigned int GLGraphEdge::getSourceX() const
{
	return vboSourceX;
}

unsigned int GLGraphEdge::getSourceY() const
{
	return vboSourceY;
}

unsigned int GLGraphEdge::getSourceZ() const
{
	return vboSourceZ;
}

unsigned int GLGraphEdge::getTargetX() const
{
	return vboTargetX;
}

unsigned int GLGraphEdge::getTargetY() const
{
	return vboTargetY;
}

unsigned int GLGraphEdge::getTargetZ() const
{
	return vboTargetZ;
}
