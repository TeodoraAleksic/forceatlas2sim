#include "clforceattr.h"
#include "kernel.h"

CLForceAttr::CLForceAttr(const cl::Device& device_, const cl::Context& context_): CLKernel(device_, context_)
{
	kernelName = "forceAttr";
	kernelBody = kernel::forceAttr;
}

CLForceAttr::~CLForceAttr()
{
}
