#ifndef _GRAPHNODE_H_
#define _GRAPHNODE_H_

#include "globject.h"

class GraphNode: public GLObject
{
private:

	static const int lats, longs;

	unsigned int vao;
	unsigned int vboVertex, vboIndex, vboOffset, vboScale;
	unsigned int program;

	void initNode(std::vector<float>* vertices, std::vector<unsigned int>* indices);

public:

	GraphNode();
	~GraphNode();

	void init();
	void draw();
	void cleanup();

};

#endif
