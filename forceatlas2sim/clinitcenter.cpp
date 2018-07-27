#include "clinitcenter.h"
#include "kernel.h"

CLInitCenter::CLInitCenter(const cl::Device& device_, const cl::Context& context_) : CLKernel(device_, context_)
{
	kernelName = "initCenter";
	kernelBody = kernel::initCenter;
}

CLInitCenter::~CLInitCenter()
{
}
