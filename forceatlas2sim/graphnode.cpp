#include "graphnode.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

const int GraphNode::lats = 40;
const int GraphNode::longs = 40;

GraphNode::GraphNode(const GraphObject& graphObject_): graphObject(graphObject_)
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

GraphNode::~GraphNode()
{
	cleanup();
}

void GraphNode::initNode(std::vector<float>* vertices, std::vector<unsigned int>* indices)
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

void GraphNode::init()
{
	// Builds shaders
	std::vector<unsigned int> shaders;
	shaders.push_back(buildShader("sphere.vert", GL_VERTEX_SHADER));
	shaders.push_back(buildShader("sphere.frag", GL_FRAGMENT_SHADER));

	// Builds program
	program = buildProgram(shaders);

	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		glDeleteShader(*iter);

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
	const std::vector<float>* x = graphObject.getNodeX();
	const std::vector<float>* y = graphObject.getNodeY();
	const std::vector<float>* z = graphObject.getNodeZ();

	glGenBuffers(1, &vboOffsetX);
	glBindBuffer(GL_ARRAY_BUFFER, vboOffsetX);
	glBufferData(GL_ARRAY_BUFFER, x->size() * sizeof(GLfloat), x, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &vboOffsetY);
	glBindBuffer(GL_ARRAY_BUFFER, vboOffsetY);
	glBufferData(GL_ARRAY_BUFFER, y->size() * sizeof(GLfloat), y, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &vboOffsetZ);
	glBindBuffer(GL_ARRAY_BUFFER, vboOffsetZ);
	glBufferData(GL_ARRAY_BUFFER, z->size() * sizeof(GLfloat), z, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(3, 1);

	// Binds degrees
	const std::vector<int>* degrees = graphObject.getNodeDegree();

	glGenBuffers(1, &vboScale);
	glBindBuffer(GL_ARRAY_BUFFER, vboScale);
	glBufferData(GL_ARRAY_BUFFER, degrees->size() * sizeof(GLfloat), degrees, GL_STATIC_DRAW);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(4, 1);

	isInited = true;
}

void GraphNode::draw()
{
}

void GraphNode::cleanup()
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
