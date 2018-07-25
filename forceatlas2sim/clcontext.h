#ifndef _CLCONTEXT_H_
#define _CLCONTEXT_H_

#include "clbase.h"

class CLContext: public CLBase
{
private:

	cl::Device device;
	cl::Context context;

	void init();

public:

	CLContext();
	~CLContext();

	const cl::Device& getDevice() const;
	const cl::Context& getContext() const;

};

#endif
