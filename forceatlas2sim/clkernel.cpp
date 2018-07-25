#include "clkernel.h"

CLKernel::CLKernel(const cl::Device& device_, const cl::Context& context_): 
	device(device_), context(context_)
{
	isInited = false;
	localWorkSize = 0;
	globalWorkSize = 0;
}

CLKernel::~CLKernel()
{
}

void CLKernel::build()
{
	try
	{
		// Builds CL kernel from source and initializes command queue
		cl::Program::Sources sources(1, std::make_pair(kernelBody.c_str(), kernelBody.length()));
		program = cl::Program(context, sources);
		program.build({ device });	
		kernel = cl::Kernel(program, kernelName.c_str());
		queue = cl::CommandQueue(context, device);
	}
	catch (cl::Error error) {
		// Handles build error
		std::cout << kernelName << " " << getErrorCode(error.err()) << " " << error.what() << "\n";
  		std::string strDirect = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
		std::cout << strDirect << "\n";
	}
}

void CLKernel::init()
{
	if (isInited)
		return;

	build();

	isInited = true;
}

void CLKernel::run()
{
	if (!isInited)
		return;

	queue.enqueueAcquireGLObjects(&glBuffers);

	try
	{
		// Runs kernel
		queue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));
	}
	catch (cl::Error error) {
		std::cout << getErrorCode(error.err()) << " " << error.what() << "\n";
	}

	queue.enqueueReleaseGLObjects(&glBuffers);
	queue.finish();
}

void CLKernel::setWorkSize(unsigned int ndRange)
{
	globalWorkSize = (ndRange % 64) > 0 ? 64 * ((int)std::ceil(ndRange / 64) + 1) : ndRange;
	localWorkSize = 64;
}

void CLKernel::setArg(unsigned int argId, GLuint glBufferId, cl_mem_flags memFlags)
{
	try
	{
		cl::BufferGL glBuffer = cl::BufferGL(context, memFlags, glBufferId, nullptr);
		glBuffers.push_back(glBuffer);
		kernel.setArg(argId, glBuffer);
	}
	catch (cl::Error error)
	{ 
		std::cout << getErrorCode(error.err()) << " " << error.what() << std::endl;
	}
}

void CLKernel::setArg(unsigned int argId, cl::Buffer clBuffer)
{
	try
	{
		clBuffers.push_back(clBuffer);
		kernel.setArg(argId, clBuffer);
	}
	catch (cl::Error error)
	{
		std::cout << getErrorCode(error.err()) << " " << error.what() << std::endl;
	}
}
