#include "clupdatenode.h"
#include "kernel.h"

CLUpdateNode::CLUpdateNode(const cl::Device& device_, const cl::Context& context_, bool fg, bool fsg): 
	CLKernel(device_, context_)
{
	if (fg)
	{
		kernelName = "updateNodeFg";
		kernelBody = kernel::updateNodeFg;
	}
	else if (fsg)
	{
		kernelName = "updateNodeFsg";
		kernelBody = kernel::updateNodeFsg;
	}
	else
	{
		kernelName = "updateNode";
		kernelBody = kernel::updateNode;
	}
}

CLUpdateNode::~CLUpdateNode()
{
}
