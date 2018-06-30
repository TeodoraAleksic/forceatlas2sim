#ifndef _CLUPDATEEDGE_H_
#define _CLUPDATEEDGE_H_

#include "clobject.h"

class CLUpdateEdge : public CLObject
{
public:

	CLUpdateEdge(const cl::Device& device_, const cl::Context& context_);
	~CLUpdateEdge();

};

#endif
