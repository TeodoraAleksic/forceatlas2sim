#ifndef _GRAPHNODE_H_
#define _GRAPHNODE_H_

#include "globject.h"

class GraphNode: public GLObject
{
public:

	GraphNode();
	~GraphNode();

	void init();
	void draw();
	void cleanup();

};

#endif
