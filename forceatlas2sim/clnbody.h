#ifndef _CLNBODY_H_
#define _CLNBODY_H_

#include "clkernel.h"

class CLNbody: public CLKernel
{
public:

	CLNbody(const cl::Device& device_, const cl::Context& context_);
	~CLNbody();

};

#endif
