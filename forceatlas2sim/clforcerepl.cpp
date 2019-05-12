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
	unsigned int numOfCUs = device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
	unsigned int minWorkGroupSize = device.getInfo<CL_DEVICE_ADDRESS_BITS>();
	unsigned int maxWorkGroupSize = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

	unsigned int ratio = (unsigned int)ceil(ndRange / minWorkGroupSize);
	unsigned int multiplier = 1;

	// Increase group size while utilizing all compute units
	while (ratio > 2 * numOfCUs)
	{
		multiplier *= 2;
		ratio = (unsigned int)ceil(ndRange / (multiplier * minWorkGroupSize));
	}

	localWorkSize = multiplier * minWorkGroupSize;
	globalWorkSize = (ndRange % localWorkSize) > 0 ?
		localWorkSize * ((int)std::ceil(ndRange / localWorkSize) + 1) : ndRange;
}
