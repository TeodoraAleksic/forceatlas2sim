#ifndef  _CLCMDQUEUE_H_
#define  _CLCMDQUEUE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "clkernel.h"

/**
* OpenCL command queue
*
* Initializes OpenCL command queue.
* Acquires and releases shared buffers.
* Runs OpenCL kernel.
*/
class CLQueue : public CLBase
{
private:

	// OpenCL device
	const cl::Device& device;
	// OpenCL context
	const cl::Context& context;

	// OpenCL command queue
	cl::CommandQueue queue;

public:

	CLQueue(const cl::Device& device_, const cl::Context& context_);
	~CLQueue();

	// Gets OpenCL command queue
	const cl::CommandQueue& getQueue() const;

	/**
	* Acquires shared buffers from OpenGL to run OpenCL kernel
	*
	* @param sharedBuffers Array of shared buffers
	*/
	void acquireGLBuffers(const std::vector<cl::Memory>& sharedBuffers);

	/**
	* Releases shared buffers held by OpenCL kernel
	*
	* @param sharedBuffers Array of shared buffers
	*/
	void releaseGLBuffers(const std::vector<cl::Memory>& sharedBuffers);

	/**
	* Runs OpenCL kernel
	*
	* @param kernel OpenCL kernel
	*/
	void runKernel(const CLKernel& kernel);

};

#endif
