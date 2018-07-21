#ifndef _CLINITCENTER_H_
#define _CLINITCENTER_H_

#include "clobject.h"

class CLInitCenter : public CLObject
{
public:

	CLInitCenter(const cl::Device& device_, const cl::Context& context_);
	~CLInitCenter();

};

#endif
