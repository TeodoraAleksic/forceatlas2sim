#ifndef _CLNBODY_H_
#define _CLNBODY_H_

#include "clobject.h"

class CLNbody: public CLObject
{
public:

	CLNbody(const cl::Device& device_, const cl::Context& context_);
	~CLNbody();

};

#endif
