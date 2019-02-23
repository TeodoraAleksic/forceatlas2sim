#ifndef  _CLCMDQUEUE_H_
#define  _CLCMDQUEUE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "clkernel.h"

class CLQueue : public CLBase
{
private:

	const cl::Device& device;
	const cl::Context& context;

	cl::CommandQueue queue;

public:

	CLQueue(const cl::Device& device_, const cl::Context& context_);
	~CLQueue();

	const cl::CommandQueue& getQueue() const;

	void acquireGLBuffers(const std::vector<cl::Memory>& sharedBuffers);
	void releaseGLBuffers(const std::vector<cl::Memory>& sharedBuffers);

	void runKernel(const CLKernel& kernel);

};

#endif
