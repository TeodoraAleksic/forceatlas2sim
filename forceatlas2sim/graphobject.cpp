#include "graphobject.h"

#include <stdlib.h>
#include <time.h>

GraphObject::GraphObject()
{
	numOfNodes = 0;
	maxDegree = 0;
	initedGraphics = false;
}

GraphObject::~GraphObject()
{
}

void GraphObject::incDegree(std::string node)
{
	int nodeIndex = nodeIds[node];
	++degree[nodeIndex];

	if (degree[nodeIndex] > maxDegree)
		maxDegree = degree[nodeIndex];
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
		srand((unsigned int)time(0));

		// Generates random node coordinates if none were provided in the graph file
		for (int i = 0; i < numOfNodes; ++i)
		{
			x[i] = rand() % (int)((float)(maxDegree * numOfNodes) / 3) - ((float)(maxDegree * numOfNodes) / 6);
			y[i] = rand() % (int)((float)(maxDegree * numOfNodes) / 3) - ((float)(maxDegree * numOfNodes) / 6);
			z[i] = rand() % (int)((float)(maxDegree * numOfNodes) / 3) - ((float)(maxDegree * numOfNodes) / 6);
		}
	}
}

int GraphObject::getNumOfNodes() const
{
	return numOfNodes;
}

const std::vector<float>* GraphObject::getNodeX() const
{
	return &x;
}

const std::vector<float>* GraphObject::getNodeY() const
{
	return &y;
}

const std::vector<float>* GraphObject::getNodeZ() const
{
	return &z;
}

const std::vector<int>* GraphObject::getNodeDegree() const
{
	return &degree;
}
