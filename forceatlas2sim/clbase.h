#ifndef _CLBASE_H_
#define _CLBASE_H_

#include <iostream>
#include <string>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.h>
#include <CL/cl.hpp>

class CLBase
{
protected:

	std::string getErrorCode(cl_int error);

	void print(cl::Platform platform);
	void print(cl::Device device);

public:

	CLBase();
	~CLBase();

};

#endif
