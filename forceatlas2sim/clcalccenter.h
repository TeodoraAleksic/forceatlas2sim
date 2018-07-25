#ifndef _CLCALCCENTER_H_
#define _CLCALCCENTER_H_

#include "clkernel.h"

class CLCalcCenter : public CLKernel
{
private:

	unsigned int maxWorkGroupSize;

public:

	CLCalcCenter(const cl::Device& device_, const cl::Context& context_);
	~CLCalcCenter();

	unsigned int getMaxWorkGroupSize();

	void setWorkSize(unsigned int ndRange);

};

#endif
