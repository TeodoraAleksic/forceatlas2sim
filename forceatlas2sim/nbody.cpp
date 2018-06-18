#include "nbody.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

NBody::NBody()
{
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
	queue = cl::CommandQueue(context, device);

	kernelCalc = buildProgram("nbodycalc.cl", "nbodyCalc");
	kernelUpdate = buildProgram("nbodyupdate.cl", "nbodyUpdate");

	isInited = true;
}

void NBody::run()
{
	if (!isSet) 
		return;

	queue.enqueueAcquireGLObjects(&glBuffers);

	try
	{
		cl::Buffer fx = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * numOfBodies);
		cl::Buffer fy = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * numOfBodies);
		cl::Buffer fz = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * numOfBodies);

		kernelCalc.setArg(0, numOfBodies);
		kernelCalc.setArg(1, x);
		kernelCalc.setArg(2, y);
		kernelCalc.setArg(3, z);
		kernelCalc.setArg(4, degree);
		kernelCalc.setArg(5, fx);
		kernelCalc.setArg(6, fy);
		kernelCalc.setArg(7, fz);

		// Runs calculate kernel
		queue.enqueueNDRangeKernel(kernelCalc, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));

		kernelUpdate.setArg(0, numOfBodies);
		kernelUpdate.setArg(1, x);
		kernelUpdate.setArg(2, y);
		kernelUpdate.setArg(3, z);
		kernelUpdate.setArg(4, degree);
		kernelUpdate.setArg(5, fx);
		kernelUpdate.setArg(6, fy);
		kernelUpdate.setArg(7, fz);

		// Runs update kernel
		queue.enqueueNDRangeKernel(kernelUpdate, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));
	}
	catch (cl::Error error) {
		std::cout << getErrorCode(error.err()) << " " << error.what() << "\n";
	}

	queue.enqueueReleaseGLObjects(&glBuffers);
	queue.finish();
}

void NBody::setArguments(const GraphNode& graphNode)
{
	if (!isInited) 
		return;

	numOfBodies = graphNode.getNumOfNodes();
	globalWorkSize = (numOfBodies % 64) > 0 ? 64 * ((int)std::ceil(numOfBodies / 64) + 1) : numOfBodies;
	localWorkSize = 64;

	x = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetX(), nullptr);
	y = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetY(), nullptr);
	z = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetZ(), nullptr);
	degree = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getScale(), nullptr);
	
	glBuffers.push_back(x);
	glBuffers.push_back(y);
	glBuffers.push_back(z);
	glBuffers.push_back(degree);

	isSet = true;
}
