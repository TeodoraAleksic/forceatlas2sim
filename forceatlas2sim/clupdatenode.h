#ifndef _CLUPDATENODE_H_
#define _CLUPDATENODE_H_

#include "clobject.h"

#include "glgraphnode.h"

class CLUpdateNode: public CLObject
{
private:

	bool isInited, isSet;

	std::vector<cl::Memory> glBuffers;
	cl::BufferGL nodeX, nodeY, nodeZ, degree;

public:

	CLUpdateNode();
	~CLUpdateNode();

	void init();
	void run();

	void setArguments(const GLGraphNode& graphNode);

};

#endif
