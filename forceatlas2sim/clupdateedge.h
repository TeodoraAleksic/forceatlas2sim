#ifndef _CLUPDATEEDGE_H_
#define _CLUPDATEEDGE_H_

#include "clobject.h"

#include "glgraphedge.h"
#include "glgraphnode.h"
#include "graphobject.h"

class CLUpdateEdge : public CLObject
{
private:

	bool isInited, isSet;

	std::vector<cl::Memory> glBuffers;
	cl::BufferGL nodeX, nodeY, nodeZ;
	cl::BufferGL sourceX, sourceY, sourceZ;
	cl::BufferGL targetX, targetY, targetZ;

	cl::Buffer sourceId, targetId;

public:

	CLUpdateEdge();
	~CLUpdateEdge();

	void init();
	void run();

	void setArguments(const GraphObject& graphObject, const GLGraphNode& graphNode, const GLGraphEdge& graphEdge);
};

#endif
