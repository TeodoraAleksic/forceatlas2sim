#ifndef _CLNBODY_H_
#define _CLNBODY_H_

#include "clobject.h"

#include "glgraphnode.h"

class CLNbody: public CLObject
{
private:

	bool isInited, isSet;

	std::vector<cl::Memory> glBuffers;
	cl::BufferGL nodeX, nodeY, nodeZ, degree;

public:

	CLNbody();
	~CLNbody();

	void init();
	void run();

	void setArguments(const GLGraphNode& graphNode);

};

#endif