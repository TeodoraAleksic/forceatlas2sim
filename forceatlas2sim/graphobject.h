#ifndef _GRAPHOBJECT_H_
#define _GRAPHOBJECT_H_

#include <string>
#include <vector>
#include <unordered_map>

class GraphObject
{
private:

	int numOfNodes;
	int maxDegree;

	bool initedGraphics;

	std::unordered_map<std::string, int> nodeIds;

	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> z;

	std::vector<int> degree;

	void incDegree(std::string node);

public:

	GraphObject();
	~GraphObject();

	void addNode(std::string node, float x_ = 0.0, float y_ = 0.0, float z_ = 0.0);
	void addEdge(std::string source, std::string target, float weight = 0.0);

	void postprocessing();

};

#endif
