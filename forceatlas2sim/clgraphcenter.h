#ifndef _CLINITCENTER_H_
#define _CLINITCENTER_H_

#include "clkernel.h"

/**
* Kernel for calculating the center of the graph
*/
class CLGraphCenter : public CLKernel
{
public:

	CLGraphCenter(const cl::Device& device_, const cl::Context& context_);
	~CLGraphCenter();

};

#endif
