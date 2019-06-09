#ifndef _CLFORCEREPL_H_
#define _CLFORCEREPL_H_

#include "clkernel.h"

/**
* Kernel for calculating the force of repulsion
*/
class CLForceRepl : public CLKernel
{
public:

	CLForceRepl(const cl::Device& device_, const cl::Context& context_);
	~CLForceRepl();

	/**
	* Sets the local and global work size.
	* Overrides base class definition.
	*
	* @param ndRange Size of the input data
	*/
	void setWorkSize(unsigned int ndRange);

};

#endif
