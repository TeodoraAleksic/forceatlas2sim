#ifndef _GRAPHNODE_H_
#define _GRAPHNODE_H_

#include "globject.h"
#include "graphobject.h"

class GraphNode: public GLObject
{
private:

	static const int lats, longs, numOfIndices;

	const GraphObject& graphObject;

	bool isInited;

	unsigned int vao;
	unsigned int vboVertex, vboIndex, vboOffsetX, vboOffsetY, vboOffsetZ, vboScale;
	unsigned int program;

	void initNode(std::vector<float>* vertices, std::vector<unsigned int>* indices);

public:

	GraphNode(const GraphObject& graphObject_);
	~GraphNode();

	void init();
	void draw();
	void cleanup();

};

#endif
