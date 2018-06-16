#ifndef _NBODY_H_
#define _NBODY_H_

#include "clobject.h"

class NBody: public CLObject
{
private:

	int localWorkSize, globalWorkSize;

	std::vector<cl::Memory> glBuffers;

public:

	NBody();
	~NBody();

	void init();
	void run();

};

#endif
