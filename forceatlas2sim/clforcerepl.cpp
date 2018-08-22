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
