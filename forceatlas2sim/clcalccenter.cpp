#include "clcalccenter.h"
#include "kernel.h"

CLCalcCenter::CLCalcCenter(const cl::Device& device_, const cl::Context& context_) : CLObject(device_, context_)
{
	kernelName = "calcCenter";
	kernelBody = kernel::calcCenter;
	maxWorkGroupSize = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
}

CLCalcCenter::~CLCalcCenter()
{
}

unsigned int CLCalcCenter::getMaxWorkGroupSize()
{
	return maxWorkGroupSize;
}

void CLCalcCenter::setWorkSize(unsigned int ndRange)
{
	globalWorkSize = (ndRange % maxWorkGroupSize) > 0 ? 
		maxWorkGroupSize * ((int)std::ceil(ndRange / maxWorkGroupSize) + 1) : ndRange;
	localWorkSize = maxWorkGroupSize;
}
