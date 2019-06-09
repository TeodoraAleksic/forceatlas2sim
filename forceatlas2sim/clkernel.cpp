#include "clkernel.h"

CLKernel::CLKernel(const cl::Device& device_, const cl::Context& context_):
	isInited(false),
	device(device_), context(context_),
	localWorkSize(0), globalWorkSize(0)
{
}

CLKernel::~CLKernel()
{
}

void CLKernel::init()
{
	if (isInited)
		return;

	try
	{
		// Builds CL kernel from source
		cl::Program::Sources sources(1, std::make_pair(kernelBody.c_str(), kernelBody.length()));
		program = cl::Program(context, sources);
		program.build({ device });
		kernel = cl::Kernel(program, kernelName.c_str());
	}
	catch (cl::Error error) {
		spdlog::error(fmt::format(msg::ERR_CL_KERNEL_BUILD, kernelName, getErrorMessage(error.err())));
		spdlog::error(error.what());
		spdlog::error(program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device));
		throw;
	}

	isInited = true;
}

std::string CLKernel::getKernelName() const
{
	return kernelName;
}

const cl::Kernel& CLKernel::getKernel() const
{
	return kernel;
}

int CLKernel::getLocalWorkSize() const
{
	return localWorkSize;
}

int CLKernel::getGlobalWorkSize() const
{
	return globalWorkSize;
}

void CLKernel::setWorkSize(unsigned int ndRange)
{
	// Gets device's minimum workgroup size
	unsigned int minWorkGroupSize = device.getInfo<CL_DEVICE_ADDRESS_BITS>();

	globalWorkSize = (ndRange % minWorkGroupSize) > 0 ? 
		minWorkGroupSize * ((int)std::ceil(ndRange / minWorkGroupSize) + 1) : ndRange;
	localWorkSize = minWorkGroupSize;
}

void CLKernel::setArg(unsigned int argId, const cl::BufferGL& glBuffer)
{
	try
	{
		kernel.setArg(argId, glBuffer);
	}
	catch (cl::Error error)
	{ 
		spdlog::error(fmt::format(msg::ERR_CL_KERNEL_ARG, 
			argId, kernelName, getErrorMessage(error.err())));
		spdlog::error(error.what());
		throw;
	}
}

void CLKernel::setArg(unsigned int argId, const cl::Buffer& clBuffer)
{
	try
	{
		kernel.setArg(argId, clBuffer);
	}
	catch (cl::Error error)
	{
		spdlog::error(fmt::format(msg::ERR_CL_KERNEL_ARG, 
			argId, kernelName, getErrorMessage(error.err())));
		spdlog::error(error.what());
		throw;
	}
}
