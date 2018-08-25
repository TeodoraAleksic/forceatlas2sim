#ifndef _CLFORCEATTR_H_
#define _CLFORCEATTR_H_

#include "clkernel.h"

class CLForceAttr: public CLKernel
{
public:

	CLForceAttr(const cl::Device& device_, const cl::Context& context_);
	~CLForceAttr();

};

#endif
