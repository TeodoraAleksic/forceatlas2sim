#ifndef _CLCALCCENTER_H_
#define _CLCALCCENTER_H_

#include "clkernel.h"

/**
* Kernel for calculating a generic sum
*/
class CLSum : public CLKernel
{
public:

	CLSum(const cl::Device& device_, const cl::Context& context_);
	~CLSum();

	void setWorkSize(unsigned int ndRange);

};

#endif
