#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "clcontext.h"

CLContext::CLContext()
{
	init();
}

CLContext::~CLContext()
{
}

void CLContext::init()
{
	unsigned int numOfPlatforms;
	clGetPlatformIDs(0, nullptr, &numOfPlatforms);

	std::vector<cl_platform_id> platforms;
	platforms.resize(numOfPlatforms);
	clGetPlatformIDs(numOfPlatforms, &platforms[0], nullptr);

	auto platform = platforms.begin();
	cl_device_id deviceId;

	for (; platform != platforms.end(); ++platform) 
	{
		cl_context_properties properties[] = {
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)(*platform),
			0
		};

		clGetGLContextInfoKHR_fn clGetGLContextInfoKHR =
			(clGetGLContextInfoKHR_fn)clGetExtensionFunctionAddressForPlatform(*platform, "clGetGLContextInfoKHR");

		cl_int errorCode = clGetGLContextInfoKHR(
			properties, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id), &deviceId, nullptr);

		if (errorCode == CL_SUCCESS)
		{
			print(cl::Platform(*platform));
			print(cl::Device(deviceId));

			device = cl::Device(deviceId);
			context = cl::Context(device , properties);

			return;
		}
	}

	std::cout << "Can't find CL/GL compatible device" << std::endl;
	throw "Can't find CL/GL compatible device";
}

const cl::Device& CLContext::getDevice() const
{
	return device;
}

const cl::Context& CLContext::getContext() const
{
	return context;
}
