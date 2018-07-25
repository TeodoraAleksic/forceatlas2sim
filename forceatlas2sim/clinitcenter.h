#ifndef _CLINITCENTER_H_
#define _CLINITCENTER_H_

#include "clkernel.h"

class CLInitCenter : public CLKernel
{
public:

	CLInitCenter(const cl::Device& device_, const cl::Context& context_);
	~CLInitCenter();

};

#endif
