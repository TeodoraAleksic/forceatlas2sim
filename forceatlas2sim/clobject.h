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

	bool isInited;

	cl::Platform platform;
	cl::Device device;

	std::string kernelName, funcName;

	cl::Program program;
	cl::Kernel kernel;

	cl::Context context;
	cl::CommandQueue cmdQueue;

	std::string getErrorCode(cl_int error);

	void printPlatform(cl::Platform platform);
	void printDevice(cl::Device device);

	void buildProgram();

public:

	CLObject();
	~CLObject();

	virtual void init() = 0;
	virtual void run() = 0;

};

#endif
