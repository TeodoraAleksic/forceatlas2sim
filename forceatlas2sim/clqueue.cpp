#include "clqueue.h"

CLQueue::CLQueue(const cl::Device& device_, const cl::Context& context_):
	device(device_), context(context_)
{
	try
	{
		queue = cl::CommandQueue(context, device);
	}
	catch (cl::Error error) {
		std::cout << getErrorCode(error.err()) << " " << error.what() << "\n";
		throw;
	}
}

CLQueue::~CLQueue()
{
}

const cl::CommandQueue& CLQueue::getQueue() const
{
	return queue;
}

void CLQueue::acquireGLBuffers(const std::vector<cl::Memory>& sharedBuffers)
{
	queue.enqueueAcquireGLObjects(&sharedBuffers);
}

void CLQueue::releaseGLBuffers(const std::vector<cl::Memory>& sharedBuffers)
{
	try
	{
		queue.enqueueReleaseGLObjects(&sharedBuffers);
		queue.finish();
	}
	catch (cl::Error error) {
		std::cout << getErrorCode(error.err()) << " " << error.what() << "\n";
	}
}

void CLQueue::runKernel(const CLKernel& kernel)
{
	try
	{
		queue.enqueueNDRangeKernel(
			kernel.getKernel(), 
			cl::NDRange(), 
			cl::NDRange(kernel.getGlobalWorkSize()), 
			cl::NDRange(kernel.getLocalWorkSize()));
	}
	catch (cl::Error error) {
		std::cout << getErrorCode(error.err()) << " " << error.what() << "\n";
	}
}
