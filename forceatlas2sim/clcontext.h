#ifndef _CLCONTEXT_H_
#define _CLCONTEXT_H_

#include "clbase.h"

class CLContext: public CLBase
{
private:

	cl::Device device;
	cl::Context context;

	// Device information
	unsigned int numOfCUs;
	unsigned int minWorkGroupSize, maxWorkGroupSize;
	unsigned long localMemSize;

	void init();

public:

	CLContext();
	~CLContext();

	const cl::Device& getDevice() const;
	const cl::Context& getContext() const;

	unsigned int getNumOfCUs() const;
	unsigned int getMinWorkGroupSize() const;
	unsigned int getMaxWorkGroupSize() const;
	unsigned long getLocalMemSize() const;

};

#endif
