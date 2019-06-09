#ifndef _CLBASE_H_
#define _CLBASE_H_

#include <iostream>
#include <string>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.h>
#include <CL/cl.hpp>

/**
* Base OpenCL class
*
* Contains functionalities used by
* all other OpenCL classes.
*/
class CLBase
{
protected:

	/**
	* Gets message corresponding to OpenCL error code
	*
	* @param error OpenCL error code
	* @return OpenCL error message
	*/
	std::string getErrorMessage(cl_int error);

	/**
	* Prints information about an OpenCL platform
	*
	* @param platform OpenCL platform
	*/
	void print(cl::Platform platform);

	/**
	* Prints information about an OpenCL device
	*
	* @param device OpenCL device
	*/
	void print(cl::Device device);

public:

	CLBase();
	~CLBase();

};

#endif
