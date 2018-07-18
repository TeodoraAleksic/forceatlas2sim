#include "graphobject.h"

GraphObject::GraphObject()
{
	numOfNodes = 0;
	numOfEdges = 0;
	initedGraphics = false;
	meanDegree = 0.0;
}

GraphObject::~GraphObject()
{
}

void GraphObject::addNode(std::string node, float x, float y, float z)
{
	// Sets and increments node Id
	nodeIds[node] = numOfNodes++;

	// Sets flag if input file contains graphics info
	if (!initedGraphics && (x != 0.0 || y != 0.0 || z != 0.0))
		initedGraphics = true;
	
	// Sets node info
	nodeX.push_back(x);
	nodeY.push_back(y);
	nodeZ.push_back(z);

	degree.push_back(0);
}

void GraphObject::addEdge(std::string source, std::string target, float weight)
{
	// Gets and saves source node info
	unsigned int sourceIndex = nodeIds[source];
	++degree[sourceIndex];
	sourceId.push_back(sourceIndex);
	sourceX.push_back(nodeX[sourceIndex]);
	sourceY.push_back(nodeY[sourceIndex]);
	sourceZ.push_back(nodeZ[sourceIndex]);

	// Gets and saves target node info
	unsigned int targetIndex = nodeIds[target];
	++degree[targetIndex];
	targetId.push_back(targetIndex);
	targetX.push_back(nodeX[targetIndex]);
	targetY.push_back(nodeY[targetIndex]);
	targetZ.push_back(nodeZ[targetIndex]);

	++numOfEdges;
}

void GraphObject::postprocessing()
{
	// Initializes node positions if node were provided
	if (!initedGraphics)
	{
		// Calculates mean node degree
		for (unsigned int i = 0; i < numOfNodes; ++i)
			meanDegree += degree[i];

		meanDegree /= numOfNodes;

		float max = meanDegree * (float)pow(numOfNodes, 1.0 / 3.0); // Length of cube side
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		// Packs nodes in a cube structure
		for (unsigned int i = 0; i < numOfNodes; ++i)
		{
			// Sets node positions
			nodeX[i] = x - max / 2;
			nodeY[i] = y - max / 2;
			nodeZ[i] = z - max / 2;

			// Calculates next node positions
			z = (z + meanDegree <= max) ? (z + meanDegree) : 0.0f;
			y = (z == 0.0) ? ((y + meanDegree <= max) ? (y + meanDegree) : 0.0f) : y;
			x = (y == 0.0 && z == 0.0) ? (x + meanDegree) : x;
		}
	}
}

float  GraphObject::getInitPosition() const
{
	// Calculates initial camera position
	if (!initedGraphics)
		return meanDegree * (float)pow(numOfNodes, 1.0 / 3.0) * (-2.0f) / sin(22.5f);
	else
		return 0.0f;
}

unsigned int GraphObject::getNumOfNodes() const
{
	return numOfNodes;
}

unsigned int GraphObject::getNumOfEdges() const
{
	return numOfEdges;
}

std::vector<float> GraphObject::getNodeX() const
{
	return nodeX;
}

std::vector<float> GraphObject::getNodeY() const
{
	return nodeY;
}

std::vector<float> GraphObject::getNodeZ() const
{
	return nodeZ;
}

std::vector<unsigned int> GraphObject::getNodeDegree() const
{
	return degree;
}

std::vector<unsigned int> GraphObject::getSourceId() const
{
	return sourceId;
}

std::vector<unsigned int> GraphObject::getTargetId() const
{
	return targetId;
}

std::vector<float> GraphObject::getSourceX() const
{
	return sourceX;
}

std::vector<float> GraphObject::getSourceY() const
{
	return sourceY;
}

std::vector<float> GraphObject::getSourceZ() const
{
	return sourceZ;
}

std::vector<float> GraphObject::getTargetX() const
{
	return targetX;
}

std::vector<float> GraphObject::getTargetY() const
{
	return targetY;
}

std::vector<float> GraphObject::getTargetZ() const
{
	return targetZ;
}
