#include "clgraphcenter.h"
#include "kernel.h"

CLGraphCenter::CLGraphCenter(const cl::Device& device_, const cl::Context& context_) : CLKernel(device_, context_)
{
	kernelName = "graphCenter";
	kernelBody = kernel::graphCenter;
}

CLGraphCenter::~CLGraphCenter()
{
}
