#ifndef _CLKERNEL_H_
#define _CLKERNEL_H_

#include <iostream>
#include <string>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.h>
#include <CL/cl.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class CLKernel
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

	CLKernel(const cl::Device& device_, const cl::Context& context_);
	~CLKernel();

	void init();
	void run();

	void setWorkSize(unsigned int ndRange);

	template <class T> void setArg(unsigned int argId, T data);
	template <class T> void setArg(unsigned int argId, unsigned int size, T* data);
	template <class T> void setArg(unsigned int argId, unsigned int size, T* data, cl_mem_flags memFlags);

	void setArg(unsigned int argId, GLuint glBufferId, cl_mem_flags memFlags);
	void setArg(unsigned int argId, cl::Buffer clBuffer);

};

template <class T> void CLKernel::setArg(unsigned int argId, T data)
{
	try
	{
		kernel.setArg(argId, data);
	}
	catch (cl::Error error)
	{
		std::cout << getErrorCode(error.err()) << " " << error.what() << std::endl;
	}
}

template <class T> void CLKernel::setArg(unsigned int argId, unsigned int size,  T* data)
{
	try
	{
		kernel.setArg(argId, size * sizeof(T), data);
	}
	catch (cl::Error error)
	{
		std::cout << getErrorCode(error.err()) << " " << error.what() << std::endl;
	}
}

template <class T> void CLKernel::setArg(unsigned int argId, unsigned int size, T* data, cl_mem_flags memFlags)
{
	try
	{
		cl::Buffer clBuffer = cl::Buffer(context, memFlags, size, data);
		clBuffers.push_back(clBuffer);
		kernel.setArg(argId, clBuffer);
	}
	catch (cl::Error error)
	{
		std::cout << getErrorCode(error.err()) << " " << error.what() << std::endl;
	}
}

#endif
