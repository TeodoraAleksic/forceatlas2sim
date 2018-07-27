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
	// Gets number of available OpenCL platforms
	unsigned int numOfPlatforms;
	clGetPlatformIDs(0, nullptr, &numOfPlatforms);

	if (numOfPlatforms == 0)
	{
		std::cout << "Could not find OpenCL platform" << std::endl;
		throw "Could not find OpenCL platform";
	}

	// Gets all available OpenCL platforms
	std::vector<cl_platform_id> platformIds;
	platformIds.resize(numOfPlatforms);
	clGetPlatformIDs(numOfPlatforms, &platformIds[0], nullptr);

	for (auto platformId = platformIds.begin(); platformId != platformIds.end(); ++platformId)
	{
		cl_context_properties properties[] = {
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),	// Windows OpenGL context
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),			// Windows OpenGL HDC
			CL_CONTEXT_PLATFORM, (cl_context_properties)(*platformId),			// OpenCL platform
			0
		};

		// Gets pointer to function clGetGLContextInfoKHR
		clGetGLContextInfoKHR_fn clGetGLContextInfoKHR =
			(clGetGLContextInfoKHR_fn)clGetExtensionFunctionAddressForPlatform(*platformId, "clGetGLContextInfoKHR");

		cl_device_id deviceId;

		// Gets Id of the device associated with the current OpenGL context
		cl_int errorCode = clGetGLContextInfoKHR(
			properties, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id), &deviceId, nullptr);

		if (errorCode == CL_SUCCESS)
		{
			print(cl::Platform(*platformId));
			print(cl::Device(deviceId));

			device = cl::Device(deviceId);
			context = cl::Context(device , properties);

			return;
		}
	}

	std::cout << "Could not find CL/GL interoperable device" << std::endl;
	throw "Could not find CL/GL interoperable device";
}

const cl::Device& CLContext::getDevice() const
{
	return device;
}

const cl::Context& CLContext::getContext() const
{
	return context;
}
