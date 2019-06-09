#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "clqueue.h"
#include "message.h"

CLQueue::CLQueue(const cl::Device& device_, const cl::Context& context_):
	device(device_), context(context_)
{
	try
	{
		queue = cl::CommandQueue(context, device);
	}
	catch (cl::Error error) {
		spdlog::error(fmt::format(msg::ERR_CL_CMD_QUEUE_INIT, getErrorMessage(error.err())));
		spdlog::error(error.what());
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
	try
	{
		queue.enqueueAcquireGLObjects(&sharedBuffers);
	}
	catch (cl::Error error) {
		spdlog::error(fmt::format(msg::ERR_CL_ACQ_BUFFER, getErrorMessage(error.err())));
		spdlog::error(error.what());
		throw;
	}
}

void CLQueue::releaseGLBuffers(const std::vector<cl::Memory>& sharedBuffers)
{
	try
	{
		queue.enqueueReleaseGLObjects(&sharedBuffers);
		queue.finish();
	}
	catch (cl::Error error) {
		spdlog::error(fmt::format(msg::ERR_CL_RLS_BUFFER, getErrorMessage(error.err())));
		spdlog::error(error.what());
		throw;
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
		spdlog::error(fmt::format(msg::ERR_CL_KERNEL_RUN, 
			kernel.getKernelName(), getErrorMessage(error.err())));
		spdlog::error(error.what());
		throw;
	}
}
