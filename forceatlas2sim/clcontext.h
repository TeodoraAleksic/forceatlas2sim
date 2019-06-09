#ifndef _CLCONTEXT_H_
#define _CLCONTEXT_H_

#include "clbase.h"

/**
* OpenCL context
*
* Finds OpenCL platform and device.
* Gets device information.
* Initializes OpenCL context.
*/
class CLContext: public CLBase
{
private:

	// OpenCL device
	cl::Device device;
	// OpenCL context
	cl::Context context;

	// The device's number of compute units
	unsigned int numOfCUs;
	// The device's minimum work group size
	unsigned int minWorkGroupSize;
	// The device's maximum work group size
	unsigned int maxWorkGroupSize;
	// Size of the device's local memory
	unsigned long localMemSize;

	// Finds device and initializes context
	void init();

public:

	CLContext();
	~CLContext();

	// Gets the OpenCL device
	const cl::Device& getDevice() const;
	// Gets the OpenCL context
	const cl::Context& getContext() const;

	// Gets the device's number of compute units
	unsigned int getNumOfCUs() const;
	// Gets the device's minimum work group size
	unsigned int getMinWorkGroupSize() const;
	// Gets the device's maximum work group size
	unsigned int getMaxWorkGroupSize() const;
	// Gets the size of the device's local memory
	unsigned long getLocalMemSize() const;

};

#endif
