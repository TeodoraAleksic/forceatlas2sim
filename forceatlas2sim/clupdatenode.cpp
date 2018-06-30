#include "clupdatenode.h"

#include <iostream>

#include "kernel.h"

CLUpdateNode::CLUpdateNode(const cl::Device& device_, const cl::Context& context_): CLObject(device_, context_)
{
	kernelName = "updateNode";
	kernelBody = updateNode;
}

CLUpdateNode::~CLUpdateNode()
{
}
