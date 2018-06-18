#ifndef _CLOBJECT_H_
#define _CLOBJECT_H_

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.h>
#include <CL/cl.hpp>

#include <string>

class CLObject
{
protected:

	cl::Platform platform;
	cl::Device device;

	cl::Context context;
	cl::CommandQueue queue;

	std::string getErrorCode(cl_int error);

	void printPlatform(cl::Platform platform);
	void printDevice(cl::Device device);

	cl::Kernel buildProgram(std::string fileName, std::string kernelName);

public:

	CLObject();
	~CLObject();

	virtual void init() = 0;
	virtual void run() = 0;

};

#endif
