#ifndef _CLOBJECT_H_
#define _CLOBJECT_H_

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.h>
#include <CL/cl.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class CLObject
{
protected:

	bool isInited;

	const cl::Device& device;
	const cl::Context& context;

	std::string kernelName, kernelBody;

	cl::CommandQueue queue;
	cl::Program program;
	cl::Kernel kernel;

	int localWorkSize, globalWorkSize;

	std::vector<cl::Memory> clBuffers;
	std::vector<cl::Memory> glBuffers;

	std::string getErrorCode(cl_int error);

	void build();

public:

	CLObject(const cl::Device& device_, const cl::Context& context_);
	~CLObject();

	void init();
	void run();

	void setWorkSize(unsigned int ndRange);

	template <class T> void setArg(unsigned int argId, T data);
	template <class T> void setArg(unsigned int argId, unsigned int size, T* data, cl_mem_flags memFlags = CL_MEM_COPY_HOST_PTR);

	void setArg(unsigned int argId, GLuint glBufferId, cl_mem_flags memFlags = CL_MEM_READ_ONLY);
	void setArg(unsigned int argId, cl::Buffer clBuffer);

};

template <class T> void CLObject::setArg(unsigned int argId, T data)
{
	kernel.setArg(argId, data);
}

template <class T> void CLObject::setArg(unsigned int argId, unsigned int size, T* data, cl_mem_flags memFlags)
{
	cl::Buffer clBuffer = cl::Buffer(context, memFlags, size, data);
	clBuffers.push_back(clBuffer);
	kernel.setArg(argId, clBuffer);
}

#endif
