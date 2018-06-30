#include "clupdateedge.h"

#include <iostream>

#include "kernel.h"

CLUpdateEdge::CLUpdateEdge(const cl::Device& device_, const cl::Context& context_) : CLObject(device_, context_)
{
	kernelName = "updateEdge";
	kernelBody = updateEdge;
}

CLUpdateEdge::~CLUpdateEdge()
{
}
