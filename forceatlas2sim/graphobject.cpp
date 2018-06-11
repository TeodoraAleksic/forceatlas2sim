#include "graphobject.h"

GraphObject::GraphObject()
{
	numOfNodes = 0;
}

GraphObject::~GraphObject()
{
}

void GraphObject::incDegree(std::string node)
{
	int nodeIndex = nodeIds[node];
	++degree[nodeIndex];
}

void GraphObject::addNode(std::string node, float x_, float y_, float z_)
{
	nodeIds[node] = numOfNodes++;
	
	// TODO generate coordinates if none were given (0.0, 0.0, 0.0)
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
