#include "clnbody.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "kernel.h"

CLNbody::CLNbody(const cl::Device& device_, const cl::Context& context_): CLObject(device_, context_)
{
	kernelName = "nBody";
	kernelBody = nBody;
}

CLNbody::~CLNbody()
{
}
