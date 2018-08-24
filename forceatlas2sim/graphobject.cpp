#include <iostream>

#include "graphobject.h"

GraphObject::GraphObject()
{
	numOfNodes = 0;
	numOfEdges = 0;
	initedGraphics = false;
	meanDegree = 0.0;
	totalDegree = 0.0;
}

GraphObject::~GraphObject()
{
}

int GraphObject::findSource(unsigned int source)
{
	// Finds first element greater than or equal to source
	for (unsigned int i = 0; i < sourceId.size(); ++i)
		if (source <= sourceId[i])
			return (int)i;

	return -1;
}

int GraphObject::findTarget(int index, unsigned int source, unsigned int target)
{
	// Finds first element greater than or equal to target with corresponding source
	for (unsigned int i = index; i < targetId.size(); ++i)
		if (target <= targetId[i] || sourceId[i] != source)
			return (int)i;

	return -1;
}

int GraphObject::insertEdgeSorted(unsigned int source, unsigned int target)
{
	if (sourceId.empty())
	{
		sourceId.push_back(source);
		targetId.push_back(target);
		return 0;
	}
	else
	{
		int i = findSource(source);

		if (i == -1)
		{
			sourceId.push_back(source);
			targetId.push_back(target);
			return sourceId.size() - 1;
		}
		else if (sourceId[i] != source)
		{
			sourceId.insert(sourceId.begin() + i, source);
			targetId.insert(targetId.begin() + i, target);
			return i;
		}
		else
		{
			i = findTarget(i, source, target);

			if (i == -1)
			{
				sourceId.push_back(source);
				targetId.push_back(target);
				return sourceId.size() - 1;
			}
			else
			{
				sourceId.insert(sourceId.begin() + i, source);
				targetId.insert(targetId.begin() + i, target);
				return i;
			}
		}
	}
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
	// Gets node Ids
	unsigned int sourceIndex = nodeIds[source];
	++degree[sourceIndex];

	unsigned int targetIndex = nodeIds[target];
	++degree[targetIndex];

	// Inserts ans sorts edge
	int i = insertEdgeSorted(sourceIndex, targetIndex);

	sourceX.insert(sourceX.begin() + i, nodeX[sourceIndex]);
	sourceY.insert(sourceY.begin() + i, nodeY[sourceIndex]);
	sourceZ.insert(sourceZ.begin() + i, nodeZ[sourceIndex]);

	targetX.insert(targetX.begin() + i, nodeX[targetIndex]);
	targetY.insert(targetY.begin() + i, nodeY[targetIndex]);
	targetZ.insert(targetZ.begin() + i, nodeZ[targetIndex]);

	edgeWeight.insert(edgeWeight.begin() + i, weight);

	++numOfEdges;
}	

void GraphObject::postprocessGraphics()
{
	// Initializes node positions if node were provided
	if (!initedGraphics)
	{
		// Calculates total and mean node degree
		for (unsigned int i = 0; i < numOfNodes; ++i)
			totalDegree += degree[i];

		meanDegree = totalDegree / numOfNodes;

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

void GraphObject::postprocessEdges()
{
	unsigned int curr = 0;

	// Calculates offsets of edges belonging to a node
	for (unsigned int i = 0; i < numOfNodes; ++i)
	{
		for (; curr < numOfEdges; ++curr)
			if (i == sourceId[curr])
			{
				edgeOffset.push_back((int)curr);
				break;
			}
			else if (i < sourceId[curr] || curr == numOfEdges - 1)
			{
				edgeOffset.push_back(-1);
				break;
			}
	}
}

void GraphObject::postprocessing()
{
	postprocessGraphics();
	postprocessEdges();
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

float GraphObject::getTotalDegree() const
{
	return totalDegree;
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

std::vector<int> GraphObject::getEdgeOffset() const
{
	return edgeOffset;
}

std::vector<float> GraphObject::getEdgeWeight() const
{
	return edgeWeight;
}
