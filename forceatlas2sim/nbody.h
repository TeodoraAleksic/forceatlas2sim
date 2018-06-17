#ifndef _NBODY_H_
#define _NBODY_H_

#include "clobject.h"

#include "graphnode.h"

class NBody: public CLObject
{
private:

	bool isInited, isSet;

	unsigned int numOfBodies;

	int localWorkSize, globalWorkSize;

	std::vector<cl::Memory> glBuffers;
	cl::BufferGL x, y, z, degree;

public:

	NBody();
	~NBody();

	void init();
	void run();

	void setArguments(const GraphNode& graphNode);

};

#endif
