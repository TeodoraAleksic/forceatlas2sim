#include "graphobject.h"

#include <cmath>

GraphObject::GraphObject()
{
	numOfNodes = 0;
	initedGraphics = false;
}

GraphObject::~GraphObject()
{
}

void GraphObject::incDegree(std::string node)
{
	unsigned int nodeIndex = nodeIds[node];
	++degree[nodeIndex];
}

void GraphObject::addNode(std::string node, float x_, float y_, float z_)
{
	nodeIds[node] = numOfNodes++;

	if (!initedGraphics && (x_ != 0.0 || y_ != 0.0 || z_ != 0.0))
		initedGraphics = true;
	
	x.push_back(x_);
	y.push_back(y_);
	z.push_back(z_);

	degree.push_back(0);
}

void GraphObject::addEdge(std::string source, std::string target, float weight)
{
	incDegree(source);
	incDegree(target);
	// TODO
}

void GraphObject::postprocessing()
{
	if (!initedGraphics)
	{
		float meanDegree = 0.0;

		for (unsigned int i = 0; i < numOfNodes; ++i)
			meanDegree += degree[i];

		meanDegree /= numOfNodes;

		float max = meanDegree * (float)pow(numOfNodes, 1.0 / 3.0);
		float x_ = 0.0f;
		float y_ = 0.0f;
		float z_ = 0.0f;

		for (unsigned int i = 0; i < numOfNodes; ++i)
		{
			x[i] = x_ - max / 2;
			y[i] = y_ - max / 2;
			z[i] = z_ - max / 2;

			z_ = (z_ + meanDegree <= max) ? (z_ + meanDegree) : 0.0f;
			y_ = (z_ == 0.0) ? ((y_ + meanDegree <= max) ? (y_ + meanDegree) : 0.0f) : y_;
			x_ = (y_ == 0.0 && z_ == 0.0) ? (x_ + meanDegree) : x_;
		}
	}
}

unsigned int GraphObject::getNumOfNodes()
{
	return numOfNodes;
}

std::vector<float> GraphObject::getNodeX()
{
	return x;
}

std::vector<float> GraphObject::getNodeY()
{
	return y;
}

std::vector<float> GraphObject::getNodeZ()
{
	return z;
}

std::vector<unsigned int> GraphObject::getNodeDegree()
{
	return degree;
}
