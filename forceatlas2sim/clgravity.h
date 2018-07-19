#ifndef _CLGRAVITY_H_
#define _CLGRAVITY_H_

#include "clobject.h"

class CLGravity : public CLObject
{
private:

	unsigned int maxWorkGroupSize;

public:

	CLGravity(const cl::Device& device_, const cl::Context& context_);
	~CLGravity();

	void setWorkSize(unsigned int ndRange);

};

#endif
