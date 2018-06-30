#ifndef _CLCONTEXT_H_
#define _CLCONTEXT_H_

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.h>
#include <CL/cl.hpp>

class CLContext
{
private:

	cl::Device device;
	cl::Context context;

	void print(cl::Platform platform);
	void print(cl::Device device);

	void init();

public:

	CLContext();
	~CLContext();

	const cl::Device& getDevice() const;
	const cl::Context& getContext() const;

};

#endif
