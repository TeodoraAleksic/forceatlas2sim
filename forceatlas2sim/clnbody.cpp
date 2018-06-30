#include "clnbody.h"
#include "kernel.h"

CLNbody::CLNbody(const cl::Device& device_, const cl::Context& context_): CLObject(device_, context_)
{
	kernelName = "nBody";
	kernelBody = kernel::nBody;
}

CLNbody::~CLNbody()
{
}
