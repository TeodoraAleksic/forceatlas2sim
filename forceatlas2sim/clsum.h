#ifndef _CLCALCCENTER_H_
#define _CLCALCCENTER_H_

#include "clkernel.h"

class CLSum : public CLKernel
{
private:

	unsigned int maxWorkGroupSize;

public:

	CLSum(const cl::Device& device_, const cl::Context& context_);
	~CLSum();

	unsigned int getMaxWorkGroupSize();

	void setWorkSize(unsigned int ndRange);

};

#endif
