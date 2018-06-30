#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glgraphnode.h"
#include "shader.h"

const int GLGraphNode::lats = 40;
const int GLGraphNode::longs = 40;
const int GLGraphNode::numOfIndices = (lats + 1) * (longs + 1) * 2 + (lats + 1);

GLGraphNode::GLGraphNode(const Camera& camera_, const GraphObject& graphObject_): camera(camera_), graphObject(graphObject_)
{
	isInited = false;
	vao = 0;
	vboVertex = 0;
	vboIndex = 0;
	vboOffsetX = 0;
	vboOffsetY = 0;
	vboOffsetZ = 0;
	vboScale = 0;
	program = 0;
}

GLGraphNode::~GLGraphNode()
{
	cleanup();
}

void GLGraphNode::initNode(std::vector<float>* vertices, std::vector<unsigned int>* indices)
{
	int indexCount = 0;

	for (int i = 0; i <= lats; i++) {
		double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (int j = 0; j <= longs; j++) {
			double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			vertices->push_back((GLfloat)(x * zr0));
			vertices->push_back((GLfloat)(y * zr0));
			vertices->push_back((GLfloat)(z0));

			indices->push_back(indexCount);
			indexCount++;

			vertices->push_back((GLfloat)(x * zr1));
			vertices->push_back((GLfloat)(y * zr1));
			vertices->push_back((GLfloat)(z1));

			indices->push_back(indexCount);
			indexCount++;
		}

		indices->push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}
}

void GLGraphNode::init()
{
	if (isInited)
		return;

	// Builds shaders
	std::vector<unsigned int> shaders;
	shaders.push_back(buildShader(GL_VERTEX_SHADER, nodeVert));
	shaders.push_back(buildShader(GL_FRAGMENT_SHADER, nodeFrag));

	// Builds program
	program = buildProgram(shaders);

	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		glDeleteShader(*iter);

	// Gets uniform variable locations
	uniformProjection = glGetUniformLocation(program, "projection");
	uniformView = glGetUniformLocation(program, "view");
	uniformModel = glGetUniformLocation(program, "model");

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	// Initiates node vertices and indices
	initNode(&vertices, &indices);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Binds vertices
	glGenBuffers(1, &vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Binds indices
	glGenBuffers(1, &vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	// Binds offsets
	std::vector<float> x = graphObject.getNodeX();
	std::vector<float> y = graphObject.getNodeY();
	std::vector<float> z = graphObject.getNodeZ();

	glGenBuffers(1, &vboOffsetX);
	glBindBuffer(GL_ARRAY_BUFFER, vboOffsetX);
	glBufferData(GL_ARRAY_BUFFER, x.size() * sizeof(GLfloat), &x[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &vboOffsetY);
	glBindBuffer(GL_ARRAY_BUFFER, vboOffsetY);
	glBufferData(GL_ARRAY_BUFFER, y.size() * sizeof(GLfloat), &y[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &vboOffsetZ);
	glBindBuffer(GL_ARRAY_BUFFER, vboOffsetZ);
	glBufferData(GL_ARRAY_BUFFER, z.size() * sizeof(GLfloat), &z[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(3, 1);

	// Binds scale
	std::vector<unsigned int> scale = graphObject.getNodeDegree();

	glGenBuffers(1, &vboScale);
	glBindBuffer(GL_ARRAY_BUFFER, vboScale);
	glBufferData(GL_ARRAY_BUFFER, scale.size() * sizeof(GLuint), &scale[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(4, 1);

	isInited = true;
}

void GLGraphNode::draw()
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

	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 2 * numOfIndices, GL_UNSIGNED_INT, NULL, graphObject.getNumOfNodes());

	glFinish();
}

void GLGraphNode::cleanup()
{
	if (!isInited)
		return;

	if (vboScale)
		glDeleteBuffers(1, &vboScale);

	if (vboOffsetX)
		glDeleteBuffers(1, &vboOffsetX);

	if (vboOffsetY)
		glDeleteBuffers(1, &vboOffsetY);

	if (vboOffsetZ)
		glDeleteBuffers(1, &vboOffsetZ);

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
	vboOffsetX = 0;
	vboOffsetY = 0;
	vboOffsetZ = 0;
	vboScale = 0;
	program = 0;
}

unsigned int GLGraphNode::getNumOfNodes() const
{
	return graphObject.getNumOfNodes();
}

unsigned int GLGraphNode::getOffsetX() const
{
	return vboOffsetX;
}

unsigned int GLGraphNode::getOffsetY() const
{
	return vboOffsetY;
}

unsigned int GLGraphNode::getOffsetZ() const
{
	return vboOffsetZ;
}

unsigned int GLGraphNode::getScale() const
{
	return vboScale;
}
