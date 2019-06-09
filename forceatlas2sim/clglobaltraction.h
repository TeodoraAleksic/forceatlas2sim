#ifndef _CLGLOBALTRACTION_H_
#define _CLGLOBALTRACTION_H_

#include "clkernel.h"

/**
* Kernel for calculating the global graph traction
*/
class CLGlobalTraction : public CLKernel
{
public:

	CLGlobalTraction(const cl::Device& device_, const cl::Context& context_);
	~CLGlobalTraction();

};

#endif
