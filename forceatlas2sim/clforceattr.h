#ifndef _CLFORCEATTR_H_
#define _CLFORCEATTR_H_

#include "clkernel.h"

/**
* Kernel for calculating the force of attraction
*/
class CLForceAttr: public CLKernel
{
public:

	CLForceAttr(const cl::Device& device_, const cl::Context& context_);
	~CLForceAttr();

	/**
	* Sets the local and global work size.
	* Overrides base class definition.
	*
	* @param ndRange Size of the input data
	*/
	void setWorkSize(unsigned int ndRange);

};

#endif
