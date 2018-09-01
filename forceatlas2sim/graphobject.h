#ifndef _GRAPHOBJECT_H_
#define _GRAPHOBJECT_H_

#include <string>
#include <vector>
#include <unordered_map>

class GraphObject
{
private:

	unsigned int numOfNodes, numOfEdges;

	bool initedX, initedY, initedZ;

	std::unordered_map<std::string, unsigned int> nodeIds;

	std::vector<float> nodeX, nodeY, nodeZ;

	float meanDegree, totalDegree;

	std::vector<unsigned int> degree;

	std::vector<unsigned int> sourceId, targetId;
	std::vector<float> sourceX, sourceY, sourceZ;
	std::vector<float> targetX, targetY, targetZ;

	std::vector<int> edgeOffset;
	std::vector<float> edgeWeight;

	void initNode(std::string node);
	unsigned int getNodeId(std::string node);

	int findSource(unsigned int source);
	int findTarget(int index, unsigned int source, unsigned int target);

	int insertEdgeSorted(unsigned int source, unsigned int target);

	void postprocessGraphics();
	void postprocessEdges();

public:

	GraphObject();
	~GraphObject();

	void addNode(std::string node, float x = 0.0, float y = 0.0, float z = 0.0);
	void addEdge(std::string source, std::string target, float weight);

	void postprocessing();

	float getInitPosition() const;

	unsigned int getNumOfNodes() const;
	unsigned int getNumOfEdges() const;

	float getTotalDegree() const;

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

	std::vector<int> getEdgeOffset() const;
	std::vector<float> getEdgeWeight() const;

};

#endif
