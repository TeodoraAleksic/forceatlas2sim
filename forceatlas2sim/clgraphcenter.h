#ifndef _CLINITCENTER_H_
#define _CLINITCENTER_H_

#include "clkernel.h"

class CLGraphCenter : public CLKernel
{
public:

	CLGraphCenter(const cl::Device& device_, const cl::Context& context_);
	~CLGraphCenter();

};

#endif
