#ifndef _NBODY_H_
#define _NBODY_H_

#include "clobject.h"

#include "graphedge.h"
#include "graphobject.h"
#include "graphnode.h"

class NBody: public CLObject
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

	NBody();
	~NBody();

	void init();
	void run();

	void setArguments(GraphObject& graphObject, const GraphNode& graphNode, const GraphEdge& graphEdge);

};

#endif
