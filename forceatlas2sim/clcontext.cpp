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

void CLContext::print(cl::Platform platform)
{
	std::cout << "Name:	" << platform.getInfo<CL_PLATFORM_NAME>() << "\n";
	std::cout << "Vendor: " << platform.getInfo<CL_PLATFORM_VENDOR>() << "\n";
	std::cout << "Version: " << platform.getInfo<CL_PLATFORM_VERSION>() << "\n";
}

void CLContext::print(cl::Device device)
{
	std::cout << "Name: " << device.getInfo<CL_DEVICE_NAME>() << "\n";
	std::cout << "Vendor: " << device.getInfo<CL_DEVICE_VENDOR>() << "\n";
	std::cout << "Version: " << device.getInfo<CL_DEVICE_VERSION>() << "\n";
}

void CLContext::init()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	std::vector<cl::Device> devices;
	platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

	// Gets info about selected OpenGL device
	std::string glVendor = (const char*)glGetString(GL_VENDOR);
	std::string glRenderer = (const char*)glGetString(GL_RENDERER);

	auto deviceIter = devices.begin();

	// Searches for CL/GL compatible device
	for (; deviceIter != devices.end(); ++deviceIter) {
		std::string clDeviceName = deviceIter->getInfo<CL_DEVICE_NAME>();
		std::string clDeviceVendor = deviceIter->getInfo<CL_DEVICE_VENDOR>();

		if (glVendor.find(clDeviceVendor.substr(0, clDeviceVendor.length() - 1)) != std::string::npos &&
			glRenderer.find(clDeviceName.substr(0, clDeviceName.length() - 1)) != std::string::npos)
			break;
	}

	if (deviceIter == devices.end())
		throw "CL/GL compatible device not found";

	device = *deviceIter;
	print(device);

	cl_context_properties properties[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platforms[0]()),
		0
	};

	// Creates CL context
	context = cl::Context(device, properties);
}

const cl::Device& CLContext::getDevice() const
{
	return device;
}

const cl::Context& CLContext::getContext() const
{
	return context;
}
