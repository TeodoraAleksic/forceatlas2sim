#ifndef _CLUPDATENODE_H_
#define _CLUPDATENODE_H_

#include "clobject.h"

class CLUpdateNode: public CLObject
{
public:

	CLUpdateNode(const cl::Device& device_, const cl::Context& context_, bool fg, bool fsg);
	~CLUpdateNode();

};

#endif
