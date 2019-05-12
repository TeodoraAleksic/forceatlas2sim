#ifndef _CLFORCEREPL_H_
#define _CLFORCEREPL_H_

#include "clkernel.h"

class CLForceRepl : public CLKernel
{
public:

	CLForceRepl(const cl::Device& device_, const cl::Context& context_);
	~CLForceRepl();

	void setWorkSize(unsigned int ndRange);

};

#endif
