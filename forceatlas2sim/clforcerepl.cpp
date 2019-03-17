#include "clforcerepl.h"
#include "kernel.h"

CLForceRepl::CLForceRepl(const cl::Device& device_, const cl::Context& context_) : CLKernel(device_, context_)
{
	kernelName = "forceRepl";
	kernelBody = kernel::forceRepl;
}

CLForceRepl::~CLForceRepl()
{
}

void CLForceRepl::setWorkSize(unsigned int ndRange)
{
	globalWorkSize = (ndRange % maxWorkGroupSize) > 0 ?
		maxWorkGroupSize * ((int)std::ceil(ndRange / maxWorkGroupSize) + 1) : ndRange;
	localWorkSize = maxWorkGroupSize;
}
