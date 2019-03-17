#ifndef _CLKERNEL_H_
#define _CLKERNEL_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "clbase.h"

class CLKernel: public CLBase
{
protected:

	bool isInited;

	const cl::Device& device;
	const cl::Context& context;

	std::string kernelName, kernelBody;

	cl::Program program;
	cl::Kernel kernel;

	const unsigned int minWorkGroupSize;
	const unsigned int maxWorkGroupSize;

	int localWorkSize, globalWorkSize;

public:

	CLKernel(const cl::Device& device_, const cl::Context& context_);
	~CLKernel();

	const cl::Kernel& getKernel() const;

	void init();

	unsigned int getMinWorkGroupSize() const;
	unsigned int getMaxWorkGroupSize() const;

	int getLocalWorkSize() const;
	int getGlobalWorkSize() const;

	void setWorkSize(unsigned int ndRange);

	template <class T> void setArg(unsigned int argId, T data);
	template <class T> void setArg(unsigned int argId, unsigned int size, T* data);

	void setArg(unsigned int argId, const cl::BufferGL& glBuffer);
	void setArg(unsigned int argId, const cl::Buffer& clBuffer);

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

#endif
