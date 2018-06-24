#include "clupdateedge.h"

#include <iostream>

#include "kernel.h"

CLUpdateEdge::CLUpdateEdge()
{
}

CLUpdateEdge::~CLUpdateEdge()
{
}

void CLUpdateEdge::init()
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

	buildProgram("updateEdge", updateEdge);

	isInited = true;
}

void CLUpdateEdge::run()
{
	if (!isSet)
		return;

	queue.enqueueAcquireGLObjects(&glBuffers);

	try
	{
		kernel.setArg(0, ndRange);
		kernel.setArg(1, nodeX);
		kernel.setArg(2, nodeY);
		kernel.setArg(3, nodeZ);
		kernel.setArg(4, sourceId);
		kernel.setArg(5, sourceX);
		kernel.setArg(6, sourceY);
		kernel.setArg(7, sourceZ);
		kernel.setArg(8, targetId);
		kernel.setArg(9, targetX);
		kernel.setArg(10, targetY);
		kernel.setArg(11, targetZ);

		// Runs kernel
		queue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));
	}
	catch (cl::Error error) {
		int a = 1;
		// std::cout << getErrorCode(error.err()) << " " << error.what() << "\n";
	}

	queue.enqueueReleaseGLObjects(&glBuffers);
	queue.finish();
}

void CLUpdateEdge::setArguments(const GraphObject& graphObject, const GLGraphNode& graphNode, const GLGraphEdge& graphEdge)
{
	if (!isInited)
		return;

	ndRange = graphEdge.getNumOfEdges();
	globalWorkSize = (ndRange % 64) > 0 ? 64 * ((int)std::ceil(ndRange / 64) + 1) : ndRange;
	localWorkSize = 64;

	nodeX = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetX(), nullptr);
	nodeY = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetY(), nullptr);
	nodeZ = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetZ(), nullptr);

	sourceX = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getSourceX(), nullptr);
	sourceY = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getSourceY(), nullptr);
	sourceZ = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getSourceZ(), nullptr);

	targetX = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getTargetX(), nullptr);
	targetY = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getTargetY(), nullptr);
	targetZ = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getTargetZ(), nullptr);

	glBuffers.push_back(nodeX);
	glBuffers.push_back(nodeY);
	glBuffers.push_back(nodeZ);

	glBuffers.push_back(sourceX);
	glBuffers.push_back(sourceY);
	glBuffers.push_back(sourceZ);

	glBuffers.push_back(targetX);
	glBuffers.push_back(targetY);
	glBuffers.push_back(targetZ);

	sourceId = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint) * ndRange, &(graphObject.getSourceId())[0]);
	targetId = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint) * ndRange, &(graphObject.getTargetId())[0]);

	isSet = true;
}
