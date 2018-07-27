#include "clupdateedge.h"
#include "kernel.h"

CLUpdateEdge::CLUpdateEdge(const cl::Device& device_, const cl::Context& context_) : CLKernel(device_, context_)
{
	kernelName = "updateEdge";
	kernelBody = kernel::updateEdge;
}

CLUpdateEdge::~CLUpdateEdge()
{
}
