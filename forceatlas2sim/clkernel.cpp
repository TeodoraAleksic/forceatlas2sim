#include "clkernel.h"

CLKernel::CLKernel(const cl::Device& device_, const cl::Context& context_):
	device(device_), context(context_), minWorkGroupSize(64), maxWorkGroupSize(device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>())
{
	isInited = false;
	localWorkSize = 0;
	globalWorkSize = 0;
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
		// Handles build error
		std::cout << kernelName << " " << getErrorCode(error.err()) << " " << error.what() << "\n";
		std::string strDirect = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
		std::cout << strDirect << "\n";
	}

	isInited = true;
}

const cl::Kernel& CLKernel::getKernel() const
{
	return kernel;
}

unsigned int CLKernel::getMinWorkGroupSize() const
{
	return minWorkGroupSize;
}

unsigned int CLKernel::getMaxWorkGroupSize() const
{
	return maxWorkGroupSize;
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
		std::cout << getErrorCode(error.err()) << " " << error.what() << std::endl;
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
		std::cout << getErrorCode(error.err()) << " " << error.what() << std::endl;
	}
}
