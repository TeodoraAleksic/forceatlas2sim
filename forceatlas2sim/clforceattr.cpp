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

void CLForceAttr::setWorkSize(unsigned int ndRange)
{
	globalWorkSize = (ndRange % maxWorkGroupSize) > 0 ?
		maxWorkGroupSize * ((int)std::ceil(ndRange / maxWorkGroupSize) + 1) : ndRange;
	localWorkSize = maxWorkGroupSize;
}
