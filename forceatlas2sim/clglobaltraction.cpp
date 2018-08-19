#include "clglobaltraction.h"
#include "kernel.h"

CLGlobalTraction::CLGlobalTraction(const cl::Device& device_, const cl::Context& context_) : CLKernel(device_, context_)
{
	kernelName = "globalTraction";
	kernelBody = kernel::globalTraction;
}

CLGlobalTraction::~CLGlobalTraction()
{
}
