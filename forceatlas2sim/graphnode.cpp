#include "graphnode.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

const int GraphNode::lats = 40;
const int GraphNode::longs = 40;

GraphNode::GraphNode()
{
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
}

void GraphNode::draw()
{
}

void GraphNode::cleanup()
{
}
