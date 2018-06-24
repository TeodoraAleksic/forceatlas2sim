#include "clupdatenode.h"

#include <iostream>

#include "kernel.h"

CLUpdateNode::CLUpdateNode()
{
}

CLUpdateNode::~CLUpdateNode()
{
}

void CLUpdateNode::init()
{
	if (isInited)
		return;

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

	buildProgram("updateNode", updateNode);

	isInited = true;
}

void CLUpdateNode::run()
{
	if (!isSet)
		return;

	queue.enqueueAcquireGLObjects(&glBuffers);

	try
	{
		cl::Buffer fx = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * ndRange);
		cl::Buffer fy = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * ndRange);
		cl::Buffer fz = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * ndRange);

		kernel.setArg(0, ndRange);
		kernel.setArg(1, nodeX);
		kernel.setArg(2, nodeY);
		kernel.setArg(3, nodeZ);
		kernel.setArg(4, degree);
		kernel.setArg(5, fx);
		kernel.setArg(6, fy);
		kernel.setArg(7, fz);

		// Runs kernel
		queue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));
	}
	catch (cl::Error error) {
		std::cout << getErrorCode(error.err()) << " " << error.what() << "\n";
	}

	queue.enqueueReleaseGLObjects(&glBuffers);
	queue.finish();
}

void CLUpdateNode::setArguments(const GLGraphNode& graphNode)
{
	if (!isInited)
		return;

	ndRange = graphNode.getNumOfNodes();
	globalWorkSize = (ndRange % 64) > 0 ? 64 * ((int)std::ceil(ndRange / 64) + 1) : ndRange;
	localWorkSize = 64;

	nodeX = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetX(), nullptr);
	nodeY = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetY(), nullptr);
	nodeZ = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetZ(), nullptr);

	degree = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getScale(), nullptr);

	glBuffers.push_back(nodeX);
	glBuffers.push_back(nodeY);
	glBuffers.push_back(nodeZ);

	glBuffers.push_back(degree);

	isSet = true;
}
