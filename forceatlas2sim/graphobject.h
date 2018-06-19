#ifndef _GRAPHOBJECT_H_
#define _GRAPHOBJECT_H_

#include <string>
#include <vector>
#include <unordered_map>

class GraphObject
{
private:

	unsigned int numOfNodes, numOfEdges;

	bool initedGraphics;

	std::unordered_map<std::string, unsigned int> nodeIds;

	std::vector<float> nodeX, nodeY, nodeZ;

	std::vector<unsigned int> degree;

	std::vector<unsigned int> sourceId, targetId;
	std::vector<float> sourceX, sourceY, sourceZ;
	std::vector<float> targetX, targetY, targetZ;

public:

	GraphObject();
	~GraphObject();

	void addNode(std::string node, float x = 0.0, float y = 0.0, float z = 0.0);
	void addEdge(std::string source, std::string target, float weight = 0.0);

	void postprocessing();

	unsigned int getNumOfNodes();
	unsigned int getNumOfEdges();

	std::vector<float> getNodeX();
	std::vector<float> getNodeY();
	std::vector<float> getNodeZ();

	std::vector<unsigned int> getNodeDegree();

	std::vector<unsigned int> getSourceId();
	std::vector<unsigned int> getTargetId();

	std::vector<float> getSourceX();
	std::vector<float> getSourceY();
	std::vector<float> getSourceZ();

	std::vector<float> getTargetX();
	std::vector<float> getTargetY();
	std::vector<float> getTargetZ();

};

#endif
