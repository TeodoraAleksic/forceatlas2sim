#ifndef _CLUPDATEEDGE_H_
#define _CLUPDATEEDGE_H_

#include "clkernel.h"

class CLUpdateEdge : public CLKernel
{
public:

	CLUpdateEdge(const cl::Device& device_, const cl::Context& context_);
	~CLUpdateEdge();

};

#endif
