#include "nbody.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

NBody::NBody()
{
	kernelName = "nbody.cl";
	funcName = "nbody";
	isInited = false;
	isSet = false;
	localWorkSize = 0;
	globalWorkSize = 0;
}

NBody::~NBody()
{
}

void NBody::init()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	std::vector<cl::Device> devices;
	platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

	// Gets info about selected OpenGL device
	std::string glVendor = (const char*)glGetString(GL_VENDOR);
	std::string glRenderer = (const char*)glGetString(GL_RENDERER);

	auto deviceIter = devices.begin();

	// Searches for cl/gl compatible device
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
	printDevice(device);

	cl_context_properties properties[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platforms[0]()),
		0
	};

	context = cl::Context(device, properties);
	cmdQueue = cl::CommandQueue(context, device);

	buildProgram();

	isInited = true;
}

void NBody::run()
{
	if (!isSet) 
		return;

	cmdQueue.enqueueAcquireGLObjects(&glBuffers);

	kernel.setArg(0, x);
	kernel.setArg(1, y);
	kernel.setArg(2, z);
	kernel.setArg(3, degree);

	try
	{
		// Runs kernel
		cmdQueue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));
	}
	catch (cl::Error error) {
		std::cout << getErrorCode(error.err()) << " " << error.what() << "\n";
	}

	cmdQueue.enqueueReleaseGLObjects(&glBuffers);
	cmdQueue.finish();
}

void NBody::setArguments(const GraphNode& graphNode)
{
	if (!isInited) 
		return;

	unsigned int numOfNodes = graphNode.getNumOfNodes();
	globalWorkSize = (numOfNodes % 64) > 0 ? 64 * ((int)std::ceil(numOfNodes / 64) + 1) : numOfNodes;
	localWorkSize = 64;

	x = cl::BufferGL(context, CL_MEM_WRITE_ONLY, graphNode.getOffsetX(), nullptr);
	y = cl::BufferGL(context, CL_MEM_WRITE_ONLY, graphNode.getOffsetY(), nullptr);
	z = cl::BufferGL(context, CL_MEM_WRITE_ONLY, graphNode.getOffsetZ(), nullptr);
	degree = cl::BufferGL(context, CL_MEM_WRITE_ONLY, graphNode.getScale(), nullptr);
	
	glBuffers.push_back(x);
	glBuffers.push_back(y);
	glBuffers.push_back(z);
	glBuffers.push_back(degree);

	isSet = true;
}
