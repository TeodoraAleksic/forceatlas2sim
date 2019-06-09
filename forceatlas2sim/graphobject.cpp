#include <cstdlib>
#include <ctime>
#include <iostream>

#include <spdlog/spdlog.h>

#include "graphobject.h"
#include "message.h"

GraphObject::GraphObject():
	numOfNodes(0), numOfEdges(0),
	initedX(false), initedY(false), initedZ(false),
	meanDegree(0.0), totalDegree(0.0)
{
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

int GraphObject::findTarget(int offset, unsigned int source, unsigned int target)
{
	// Finds first element greater than or equal to target with corresponding source
	for (unsigned int i = offset; i < targetId.size(); ++i)
		if (target <= targetId[i] || sourceId[i] != source)
			return (int)i;

	return -1;
}

int GraphObject::insertEdgeSorted(unsigned int source, unsigned int target)
{
	if (sourceId.empty())
	{
		// If the edge is the first edge, just insert it into the arrays
		sourceId.push_back(source);
		targetId.push_back(target);
		return 0;
	}
	else
	{
		// Tries to find the edge's source in the array of source node Ids
		int i = findSource(source);

		if (i == -1)
		{
			// Did not find the edge's source, meaning it's larger
			// than all existing source node Ids. Inserts the edge
			// at the end of the arrays.
			sourceId.push_back(source);
			targetId.push_back(target);
			return sourceId.size() - 1;
		}
		else if (sourceId[i] != source)
		{
			// Did not find the edge's source, but found the
			// first larger source node Id. Inserts edge
			// right before the larger node.
			sourceId.insert(sourceId.begin() + i, source);
			targetId.insert(targetId.begin() + i, target);
			return i;
		}
		else
		{
			// Found the edge's source. Tries to find the
			// first larger target node than the edge's 
			// target in the array of target node Ids.
			i = findTarget(i, source, target);

			if (i == -1)
			{
				// Reached the end of the arrays. Inserts
				// new edge at the end of the arrays.
				sourceId.push_back(source);
				targetId.push_back(target);
				return sourceId.size() - 1;
			}
			else
			{
				// Found the first larger target node than the
				// edge's target. Inserts the edge right before 
				// the larger target node.
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

	// Inserts and sorts edge
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
	spdlog::info(msg::INFO_POST_PROC_GRAPHICS);

	if (!initedX && !initedY && !initedZ) // Initializes node positions if none were provided
	{
		spdlog::info(msg::INFO_PACK_NODES_IN_CUBE);

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
		if (!initedX || !initedY || !initedZ)
		{
			spdlog::info(msg::INFO_ZERO_FORCE_SUM);

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
}

void GraphObject::postprocessEdges()
{
	spdlog::info(msg::INFO_POST_PROC_EDGES);

	unsigned int curr = 0;

	// Calculates the offsets of nodes appearing in the edge source node array
	for (unsigned int i = 0; i < numOfNodes; ++i)
	{
		for (; curr < numOfEdges; ++curr)
			if (i == sourceId[curr])
			{
				sourceNodeOffset.push_back((int)curr);
				break;
			}
			else if (i < sourceId[curr] || curr == numOfEdges - 1)
			{
				sourceNodeOffset.push_back(-1);
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

std::vector<int> GraphObject::getSourceNodeOffset() const
{
	return sourceNodeOffset;
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

std::vector<float> GraphObject::getEdgeWeight() const
{
	return edgeWeight;
}
