#ifndef _CLCALCCENTER_H_
#define _CLCALCCENTER_H_

#include "clobject.h"

class CLCalcCenter : public CLObject
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
