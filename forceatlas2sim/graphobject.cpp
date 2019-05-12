#include <cstdlib>
#include <ctime>
#include <iostream>

#include "graphobject.h"

GraphObject::GraphObject()
{
	numOfNodes = 0;
	numOfEdges = 0;

	initedX = false;
	initedY = false;
	initedZ = false;

	meanDegree = 0.0;
	totalDegree = 0.0;
}

GraphObject::~GraphObject()
{
}

void GraphObject::swap(unsigned int* a, unsigned int* b)
{
	unsigned int c = *a;
	*a = *b;
	*b = c;
}

void GraphObject::initNode(std::string node)
{
	// Generates new node Id
	nodeIds[node] = numOfNodes++;

	// Initializes node position and degree
	nodeX.push_back(0);
	nodeY.push_back(0);
	nodeZ.push_back(0);
	degree.push_back(0);
	nodeLabel.push_back("");
	sourceNodeCount.push_back(0);
}

unsigned int GraphObject::getNodeId(std::string node)
{
	if (nodeIds.find(node) != nodeIds.end())
		return nodeIds[node];
	else
	{
		initNode(node);
		return (numOfNodes - 1);
	}
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

void GraphObject::addNode(std::string node, std::string label, float x, float y, float z)
{
	unsigned int nodeId = getNodeId(node);

	// Sets flag if input file contains graphics info
	if (x != 0.0) initedX = true;
	if (y != 0.0) initedY = true;
	if (z != 0.0) initedZ = true;

	nodeX[nodeId] = x;
	nodeY[nodeId] = y;
	nodeZ[nodeId] = z;

	nodeLabel[nodeId] = label;
}

void GraphObject::addEdge(std::string source, std::string target, float weight)
{
	// Gets node Ids
	unsigned int sourceIndex = getNodeId(source);
	++degree[sourceIndex];

	unsigned int targetIndex = getNodeId(target);
	++degree[targetIndex];

	// If the first node appears as the source node more often, switch the nodes
	if (sourceNodeCount[sourceIndex] > sourceNodeCount[targetIndex])
		swap(&sourceIndex, &targetIndex);

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
	++sourceNodeCount[sourceIndex];
}	

void GraphObject::postprocessGraphics()
{
	if (!initedX && !initedY && !initedZ) // Initializes node positions if none were provided
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
	else
	{
		srand((unsigned int)time(NULL));

		int min = -1;
		int max = 1;

		// Sets small random values for uninitialized axis to avoid zero force sums
		for (unsigned int i = 0; i < numOfNodes; ++i)
		{
			if (!initedX)
				nodeX[i] = (min + (rand() % (int)(max - min + 1))) / 10.0f;

			if (!initedY)
				nodeY[i] = (min + (rand() % (int)(max - min + 1))) / 10.0f;

			if (!initedZ)
				nodeZ[i] = (min + (rand() % (int)(max - min + 1))) / 10.0f;
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
	if (!initedX && !initedY && !initedZ)
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

std::string GraphObject::getNodeLabel(unsigned int id) const
{
	if (id >= 0 && id < nodeLabel.size())
		return nodeLabel[id];
	else
		return "";
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
