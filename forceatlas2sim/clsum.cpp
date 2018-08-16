#include "clsum.h"
#include "kernel.h"

CLSum::CLSum(const cl::Device& device_, const cl::Context& context_) : CLKernel(device_, context_)
{
	kernelName = "sum";
	kernelBody = kernel::sum;
	maxWorkGroupSize = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
}

CLSum::~CLSum()
{
}

unsigned int CLSum::getMaxWorkGroupSize()
{
	return maxWorkGroupSize;
}

void CLSum::setWorkSize(unsigned int ndRange)
{
	globalWorkSize = (ndRange % maxWorkGroupSize) > 0 ? 
		maxWorkGroupSize * ((int)std::ceil(ndRange / maxWorkGroupSize) + 1) : ndRange;
	localWorkSize = maxWorkGroupSize;
}
