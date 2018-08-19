#include "clupdatenode.h"
#include "kernel.h"

CLUpdateNode::CLUpdateNode(const cl::Device& device_, const cl::Context& context_): 
	CLKernel(device_, context_)
{
	kernelName = "updateNode";
	kernelBody = kernel::updateNode;
}

CLUpdateNode::~CLUpdateNode()
{
}
