#ifndef _CLGLOBALSWING_H_
#define _CLGLOBALSWING_H_

#include "clkernel.h"

class CLGlobalSwing : public CLKernel
{
public:

	CLGlobalSwing(const cl::Device& device_, const cl::Context& context_);
	~CLGlobalSwing();

};

#endif
