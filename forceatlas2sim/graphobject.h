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

	unsigned int getNumOfNodes() const;
	unsigned int getNumOfEdges() const;

	std::vector<float> getNodeX() const;
	std::vector<float> getNodeY() const;
	std::vector<float> getNodeZ() const;

	std::vector<unsigned int> getNodeDegree() const;

	std::vector<unsigned int> getSourceId() const;
	std::vector<unsigned int> getTargetId() const;

	std::vector<float> getSourceX() const;
	std::vector<float> getSourceY() const;
	std::vector<float> getSourceZ() const;

	std::vector<float> getTargetX() const;
	std::vector<float> getTargetY() const;
	std::vector<float> getTargetZ() const;

};

#endif
