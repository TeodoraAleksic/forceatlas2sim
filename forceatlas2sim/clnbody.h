#ifndef _CLNBODY_H_
#define _CLNBODY_H_

#include "clobject.h"

#include "glgraphedge.h"
#include "graphobject.h"
#include "glgraphnode.h"

class CLNbody: public CLObject
{
private:

	bool isInited, isSet;

	unsigned int numOfNodes, numOfEdges;

	int localWorkSize, globalWorkSize;

	cl::Kernel kernelCalc, kernelUpdateNode, kernelUpdateEdge;

	std::vector<cl::Memory> glBuffers;
	cl::BufferGL nodeX, nodeY, nodeZ, degree;
	cl::BufferGL sourceX, sourceY, sourceZ;
	cl::BufferGL targetX, targetY, targetZ;

	cl::Buffer sourceId, targetId;

public:

	CLNbody();
	~CLNbody();

	void init();
	void run();

	void setArguments(const GraphObject& graphObject, const GLGraphNode& graphNode, const GLGraphEdge& graphEdge);

};

#endif
