#ifndef _CLKERNEL_H_
#define _CLKERNEL_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "clbase.h"
#include "message.h"

/**
* OpenCL kernel
*
* Compiles kernel code.
* Computes local and global work size.
* Sets kernel arguments.
*/
class CLKernel: public CLBase
{
protected:

	// Indicates if the kernel has been initialized
	bool isInited;

	// OpenCL device
	const cl::Device& device;
	// OpenCL context
	const cl::Context& context;

	// Name of the kernel function
	std::string kernelName;
	// Body of the kernel function
	std::string kernelBody;

	// OpenCL program
	cl::Program program;
	// OpenCL kernel
	cl::Kernel kernel;

	// Size of the data handled by each workgroup
	int localWorkSize;
	// Total size of the data
	int globalWorkSize;

public:

	CLKernel(const cl::Device& device_, const cl::Context& context_);
	~CLKernel();

	// Compiles OpenCL kernel
	void init();

	// Gets the OpenCL kernel
	const cl::Kernel& getKernel() const;

	// Gets the name of the kernel function
	std::string getKernelName() const;

	// Gets the size of the data handled by each workgroup
	int getLocalWorkSize() const;
	// Gets the total size of the data
	int getGlobalWorkSize() const;

	/**
	* Sets the local and global work size
	*
	* @param ndRange Size of the input data
	*/
	void setWorkSize(unsigned int ndRange);

	/**
	* Sets a kernel argument.
	* Uses a single piece of data.
	*
	* @param argId	Id of the argument
	* @param data	Singular data
	*/
	template <class T> void setArg(unsigned int argId, T data);

	/**
	* Sets a kernel argument.
	* Uses a data array.
	*
	* @param argId	Id of the argument
	* @param size	Length of data array
	* @param data	Array of data
	*/
	template <class T> void setArg(unsigned int argId, unsigned int size, T* data);

	/**
	* Sets a kernel argument.
	* Uses initialized OpenGL buffer.
	*
	* @param argId		Id of the argument
	* @param glBuffer	OpenGL buffer
	*/
	void setArg(unsigned int argId, const cl::BufferGL& glBuffer);
	
	/**
	* Sets a kernel argument.
	* Uses initialized OpenCL buffer.
	*
	* @param argId		Id of the argument
	* @param clBuffer	OpenCL buffer
	*/
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
		spdlog::error(fmt::format(msg::ERR_CL_KERNEL_ARG, 
			argId, kernelName, getErrorMessage(error.err())));
		spdlog::error(error.what());
		throw;
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
		spdlog::error(fmt::format(msg::ERR_CL_KERNEL_ARG, 
			argId, kernelName, getErrorMessage(error.err())));
		spdlog::error(error.what());
		throw;
	}
}

#endif
