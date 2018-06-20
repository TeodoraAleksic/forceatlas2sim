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

	kernelCalc = buildProgram("nbodycalc.cl", "nbodyCalc");
	kernelUpdateNode = buildProgram("nbodyupdatenode.cl", "nbodyUpdateNode");
	kernelUpdateEdge = buildProgram("nbodyupdateedge.cl", "nbodyUpdateEdge");

	isInited = true;
}

void NBody::run()
{
	if (!isSet) 
		return;

	queue.enqueueAcquireGLObjects(&glBuffers);

	try
	{
		cl::Buffer fx = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * numOfNodes);
		cl::Buffer fy = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * numOfNodes);
		cl::Buffer fz = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * numOfNodes);

		kernelCalc.setArg(0, numOfNodes);
		kernelCalc.setArg(1, nodeX);
		kernelCalc.setArg(2, nodeY);
		kernelCalc.setArg(3, nodeZ);
		kernelCalc.setArg(4, degree);
		kernelCalc.setArg(5, fx);
		kernelCalc.setArg(6, fy);
		kernelCalc.setArg(7, fz);

		globalWorkSize = (numOfNodes % 64) > 0 ? 64 * ((int)std::ceil(numOfNodes / 64) + 1) : numOfNodes;
		localWorkSize = 64;

		// Runs calculate kernel
		queue.enqueueNDRangeKernel(kernelCalc, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));

		kernelUpdateNode.setArg(0, numOfNodes);
		kernelUpdateNode.setArg(1, nodeX);
		kernelUpdateNode.setArg(2, nodeY);
		kernelUpdateNode.setArg(3, nodeZ);
		kernelUpdateNode.setArg(4, degree);
		kernelUpdateNode.setArg(5, fx);
		kernelUpdateNode.setArg(6, fy);
		kernelUpdateNode.setArg(7, fz);

		globalWorkSize = (numOfNodes % 64) > 0 ? 64 * ((int)std::ceil(numOfNodes / 64) + 1) : numOfNodes;
		localWorkSize = 64;

		// Runs update nodes kernel
		queue.enqueueNDRangeKernel(kernelUpdateNode, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));

		kernelUpdateEdge.setArg(0, numOfEdges);
		kernelUpdateEdge.setArg(1, nodeX);
		kernelUpdateEdge.setArg(2, nodeY);
		kernelUpdateEdge.setArg(3, nodeZ);
		kernelUpdateEdge.setArg(4, sourceId);
		kernelUpdateEdge.setArg(5, sourceX);
		kernelUpdateEdge.setArg(6, sourceY);
		kernelUpdateEdge.setArg(7, sourceZ);
		kernelUpdateEdge.setArg(8, targetId);
		kernelUpdateEdge.setArg(9, targetX);
		kernelUpdateEdge.setArg(10, targetY);
		kernelUpdateEdge.setArg(11, targetZ);

		globalWorkSize = (numOfEdges % 64) > 0 ? 64 * ((int)std::ceil(numOfEdges / 64) + 1) : numOfEdges;
		localWorkSize = 64;

		// Runs update edges kernel
		queue.enqueueNDRangeKernel(kernelUpdateEdge, cl::NDRange(), cl::NDRange(globalWorkSize), cl::NDRange(localWorkSize));
	}
	catch (cl::Error error) {
		std::cout << getErrorCode(error.err()) << " " << error.what() << "\n";
	}

	queue.enqueueReleaseGLObjects(&glBuffers);
	queue.finish();
}

void NBody::setArguments(GraphObject& graphObject, const GraphNode& graphNode, const GraphEdge& graphEdge)
{
	if (!isInited) 
		return;

	numOfNodes = graphNode.getNumOfNodes();
	numOfEdges = graphEdge.getNumOfEdges();

	nodeX = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetX(), nullptr);
	nodeY = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetY(), nullptr);
	nodeZ = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getOffsetZ(), nullptr);

	degree = cl::BufferGL(context, CL_MEM_READ_WRITE, graphNode.getScale(), nullptr);

	sourceX = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getSourceX(), nullptr);
	sourceY = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getSourceY(), nullptr);
	sourceZ = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getSourceZ(), nullptr);

	targetX = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getTargetX(), nullptr);
	targetY = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getTargetY(), nullptr);
	targetZ = cl::BufferGL(context, CL_MEM_READ_WRITE, graphEdge.getTargetZ(), nullptr);
	
	glBuffers.push_back(nodeX);
	glBuffers.push_back(nodeY);
	glBuffers.push_back(nodeZ);

	glBuffers.push_back(degree);

	glBuffers.push_back(sourceX);
	glBuffers.push_back(sourceY);
	glBuffers.push_back(sourceZ);

	glBuffers.push_back(targetX);
	glBuffers.push_back(targetY);
	glBuffers.push_back(targetZ);

	sourceId = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint) * numOfEdges, &(graphObject.getSourceId())[0]);
	targetId = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint) * numOfEdges, &(graphObject.getTargetId())[0]);

	isSet = true;
}
