#ifndef _CLUPDATENODE_H_
#define _CLUPDATENODE_H_

#include "clkernel.h"

class CLUpdateNode: public CLKernel
{
public:

	CLUpdateNode(const cl::Device& device_, const cl::Context& context_);
	~CLUpdateNode();

};

#endif
