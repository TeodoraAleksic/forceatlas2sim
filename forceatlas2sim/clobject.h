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

	cl::Platform platform;
	cl::Device device;

	cl::Context context;
	cl::CommandQueue queue;

	cl::Program program;
	cl::Kernel kernel;

	int ndRange, localWorkSize, globalWorkSize;

	std::vector<cl::Memory> clBuffers;
	std::vector<cl::Memory> glBuffers;

	std::string getErrorCode(cl_int error);

	void printPlatform(cl::Platform platform);
	void printDevice(cl::Device device);

	void buildProgram(std::string kernelName, std::string kernelBody);

public:

	CLObject();
	~CLObject();

	virtual void init() = 0;
	virtual void run() = 0;

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
