#include "clglobalswing.h"
#include "kernel.h"

CLGlobalSwing::CLGlobalSwing(const cl::Device& device_, const cl::Context& context_) : CLKernel(device_, context_)
{
	kernelName = "globalSwing";
	kernelBody = kernel::globalSwing;
}

CLGlobalSwing::~CLGlobalSwing()
{
}
