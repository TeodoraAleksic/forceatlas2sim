#include "clgravity.h"
#include "kernel.h"

CLGravity::CLGravity(const cl::Device& device_, const cl::Context& context_) : CLObject(device_, context_)
{
	kernelName = "sum";
	kernelBody = kernel::gravity;
	maxWorkGroupSize = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
}

CLGravity::~CLGravity()
{
}

void CLGravity::setWorkSize(unsigned int ndRange)
{
	globalWorkSize = (ndRange % maxWorkGroupSize) > 0 ? 
		maxWorkGroupSize * ((int)std::ceil(ndRange / maxWorkGroupSize) + 1) : ndRange;
	localWorkSize = maxWorkGroupSize;
}
