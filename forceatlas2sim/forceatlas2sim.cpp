#include <vector>

#include "forceatlas2sim.h"

ForceAtlas2Sim::ForceAtlas2Sim(
	const ForceAtlas2Params& fa2Params_, 
	const GraphObject& graphObject_, 
	const GLGraphNode& glGraphNode_, 
	const GLGraphEdge& glGraphEdge_):
	fa2Params(fa2Params_),
	graphObject(graphObject_),
	glGraphNode(glGraphNode_),
	glGraphEdge(glGraphEdge_),
	clGlobalSwing(clContext.getDevice(), clContext.getContext()),
	clGlobalTraction(clContext.getDevice(), clContext.getContext()),
	clGraphCenter(clContext.getDevice(), clContext.getContext()),
	clSum(clContext.getDevice(), clContext.getContext()),
	clForceAttr(clContext.getDevice(), clContext.getContext()),
	clForceRepl(clContext.getDevice(), clContext.getContext()),
	clUpdateNode(clContext.getDevice(), clContext.getContext()),
	clUpdateEdge(clContext.getDevice(), clContext.getContext()),
	clQueue(clContext.getDevice(), clContext.getContext()),
	clContext()
{
	forceFront = 0;
	tmpFront = 0;
}

ForceAtlas2Sim::~ForceAtlas2Sim()
{
}

void ForceAtlas2Sim::setCLGlobalSwingArgs(bool init)
{
	if (init)
	{
		clGlobalSwing.setWorkSize(graphObject.getNumOfNodes());
		clGlobalSwing.setArg(0, graphObject.getNumOfNodes());
		clGlobalSwing.setArg(10, glNodeScale);
	}

	clGlobalSwing.setArg(1, tmpX[tmpFront]);
	clGlobalSwing.setArg(2, tmpY[tmpFront]);
	clGlobalSwing.setArg(3, tmpZ[tmpFront]);
	clGlobalSwing.setArg(4, fx[forceFront]);
	clGlobalSwing.setArg(5, fy[forceFront]);
	clGlobalSwing.setArg(6, fz[forceFront]);
	clGlobalSwing.setArg(7, fx[forceFront == 1 ? 0 : 1]);
	clGlobalSwing.setArg(8, fy[forceFront == 1 ? 0 : 1]);
	clGlobalSwing.setArg(9, fz[forceFront == 1 ? 0 : 1]);
}

void ForceAtlas2Sim::setCLGlobalTractionArgs(bool init)
{
	if (init)
	{
		clGlobalTraction.setWorkSize(graphObject.getNumOfNodes());
		clGlobalTraction.setArg(0, graphObject.getNumOfNodes());
		clGlobalTraction.setArg(10, glNodeScale);
	}

	clGlobalTraction.setArg(1, tmpX[tmpFront]);
	clGlobalTraction.setArg(2, tmpY[tmpFront]);
	clGlobalTraction.setArg(3, tmpZ[tmpFront]);
	clGlobalTraction.setArg(4, fx[forceFront]);
	clGlobalTraction.setArg(5, fy[forceFront]);
	clGlobalTraction.setArg(6, fz[forceFront]);
	clGlobalTraction.setArg(7, fx[forceFront == 1 ? 0 : 1]);
	clGlobalTraction.setArg(8, fy[forceFront == 1 ? 0 : 1]);
	clGlobalTraction.setArg(9, fz[forceFront == 1 ? 0 : 1]);
}

void ForceAtlas2Sim::setCLGraphCenterArgs(bool init)
{
	if (init)
	{
		clGraphCenter.setWorkSize(graphObject.getNumOfNodes());
		clGraphCenter.setArg(0, graphObject.getNumOfNodes());
		clGraphCenter.setArg(1, glNodeX);
		clGraphCenter.setArg(2, glNodeY);
		clGraphCenter.setArg(3, glNodeZ);
		clGraphCenter.setArg(4, glNodeScale);
	}

	clGraphCenter.setArg(5, tmpX[tmpFront]);
	clGraphCenter.setArg(6, tmpY[tmpFront]);
	clGraphCenter.setArg(7, tmpZ[tmpFront]);
}

void ForceAtlas2Sim::setCLForceAttrArgs(bool init)
{
	if (init)
	{
		clForceAttr.setWorkSize(graphObject.getNumOfNodes());
		clForceAttr.setArg(0, graphObject.getNumOfNodes());
		clForceAttr.setArg(1, graphObject.getNumOfEdges());
		clForceAttr.setArg(2, fa2Params.getDelta());
		clForceAttr.setArg(3, glNodeX);
		clForceAttr.setArg(4, glNodeY);
		clForceAttr.setArg(5, glNodeZ);
		clForceAttr.setArg(6, glNodeScale);
		clForceAttr.setArg(10, glEdgeSourceId);
		clForceAttr.setArg(11, glEdgeTargetId);
		clForceAttr.setArg(12, edgeOffset);
		clForceAttr.setArg(13, edgeWeight);
	}

	float* localFloat = nullptr;
	unsigned int* localUInt = nullptr;

	clForceAttr.setArg(7, fx[forceFront]);
	clForceAttr.setArg(8, fy[forceFront]);
	clForceAttr.setArg(9, fz[forceFront]);
	clForceAttr.setArg(14, clForceAttr.getLocalWorkSize(), localFloat);
	clForceAttr.setArg(15, clForceAttr.getLocalWorkSize(), localFloat);
	clForceAttr.setArg(16, clForceAttr.getLocalWorkSize(), localFloat);
	clForceAttr.setArg(17, clForceAttr.getLocalWorkSize(), localUInt);
}

void ForceAtlas2Sim::setCLForceReplArgs(bool init)
{
	if (init)
	{
		clForceRepl.setWorkSize(graphObject.getNumOfNodes());
		clForceRepl.setArg(0, graphObject.getNumOfNodes());
		clForceRepl.setArg(1, fa2Params.getKr());
		clForceRepl.setArg(2, fa2Params.getKrp());
		clForceRepl.setArg(3, glNodeX);
		clForceRepl.setArg(4, glNodeY);
		clForceRepl.setArg(5, glNodeZ);
		clForceRepl.setArg(6, glNodeScale);
	}

	float* localFloat = nullptr;
	unsigned int* localUInt = nullptr;

	clForceRepl.setArg(7, fx[forceFront]);
	clForceRepl.setArg(8, fy[forceFront]);
	clForceRepl.setArg(9, fz[forceFront]);
	clForceRepl.setArg(10, clForceRepl.getLocalWorkSize(), localFloat);
	clForceRepl.setArg(11, clForceRepl.getLocalWorkSize(), localFloat);
	clForceRepl.setArg(12, clForceRepl.getLocalWorkSize(), localFloat);
	clForceRepl.setArg(13, clForceRepl.getLocalWorkSize(), localUInt);
}

void ForceAtlas2Sim::setCLUpdateNodeArgs(bool init)
{
	if (init)
	{
		clUpdateNode.setWorkSize(graphObject.getNumOfNodes());
		clUpdateNode.setArg(0, graphObject.getNumOfNodes());
		clUpdateNode.setArg(1, fa2Params.getKg());
		clUpdateNode.setArg(2, graphObject.getTotalDegree());
		clUpdateNode.setArg(3, fa2Params.getFg() ? 1 : 0);
		clUpdateNode.setArg(4, fa2Params.getFsg() ? 1 : 0);
		clUpdateNode.setArg(5, fa2Params.getTau());
		clUpdateNode.setArg(6, fa2Params.getKs());
		clUpdateNode.setArg(7, fa2Params.getKsmax());
		clUpdateNode.setArg(8, centerOfMass);
		clUpdateNode.setArg(9, globalSwing);
		clUpdateNode.setArg(10, globalTraction);
		clUpdateNode.setArg(11, glNodeX);
		clUpdateNode.setArg(12, glNodeY);
		clUpdateNode.setArg(13, glNodeZ);
		clUpdateNode.setArg(14, glNodeScale);
	}

	clUpdateNode.setArg(15, fx[forceFront]);
	clUpdateNode.setArg(16, fy[forceFront]);
	clUpdateNode.setArg(17, fz[forceFront]);
	clUpdateNode.setArg(18, fx[forceFront == 1 ? 0 : 1]);
	clUpdateNode.setArg(19, fy[forceFront == 1 ? 0 : 1]);
	clUpdateNode.setArg(20, fz[forceFront == 1 ? 0 : 1]);
}

void ForceAtlas2Sim::setCLUpdateEdgeArgs(bool init)
{
	if (init)
	{
		clUpdateEdge.setWorkSize(graphObject.getNumOfEdges());
		clUpdateEdge.setArg(0, graphObject.getNumOfEdges());
		clUpdateEdge.setArg(1, glNodeX);
		clUpdateEdge.setArg(2, glNodeY);
		clUpdateEdge.setArg(3, glNodeZ);
		clUpdateEdge.setArg(4, glEdgeSourceId);
		clUpdateEdge.setArg(5, glEdgeSourceX);
		clUpdateEdge.setArg(6, glEdgeSourceY);
		clUpdateEdge.setArg(7, glEdgeSourceZ);
		clUpdateEdge.setArg(8, glEdgeTargetId);
		clUpdateEdge.setArg(9, glEdgeTargetX);
		clUpdateEdge.setArg(10, glEdgeTargetY);
		clUpdateEdge.setArg(11, glEdgeTargetZ);
	}
}

void ForceAtlas2Sim::setCLSumArgs(unsigned int n, unsigned int workGroupSize, cl::Buffer graphGlobal)
{
	float* partial = nullptr;

	clSum.setWorkSize(n);
	clSum.setArg(0, n);
	clSum.setArg(1, graphGlobal);
	clSum.setArg(2, tmpX[tmpFront]);
	clSum.setArg(3, tmpY[tmpFront]);
	clSum.setArg(4, tmpZ[tmpFront]);
	clSum.setArg(5, tmpX[tmpFront == 1 ? 0 : 1]);
	clSum.setArg(6, tmpY[tmpFront == 1 ? 0 : 1]);
	clSum.setArg(7, tmpZ[tmpFront == 1 ? 0 : 1]);
	clSum.setArg(8, workGroupSize, partial);
	clSum.setArg(9, workGroupSize, partial);
	clSum.setArg(10, workGroupSize, partial);
}

void ForceAtlas2Sim::sum(unsigned int n, cl::Buffer graphGlobal)
{
	unsigned int workGroupSize = clContext.getMaxWorkGroupSize();

	setCLSumArgs(n, workGroupSize, graphGlobal);
	clQueue.runKernel(clSum);
	tmpFront = tmpFront == 1 ? 0 : 1;

	// Sums partial sums
	while (n > workGroupSize)
	{
		n = (unsigned int)std::ceil(n / workGroupSize);
		setCLSumArgs(n, workGroupSize, graphGlobal);
		clQueue.runKernel(clSum);
		tmpFront = tmpFront == 1 ? 0 : 1;
	}
}

void ForceAtlas2Sim::init()
{
	{
		std::vector<int> zeros(3, 0);

		// Allocates buffers for global variables
		centerOfMass = cl::Buffer(clContext.getContext(), 
			CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 3, &zeros[0]);
		globalSwing = cl::Buffer(clContext.getContext(), 
			CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 3, &zeros[0]);
		globalTraction = cl::Buffer(clContext.getContext(), 
			CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 3, &zeros[0]);
	}

	// Allocates buffers for values used for calculation
	edgeOffset = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		sizeof(cl_int) * graphObject.getNumOfNodes(), &(graphObject.getEdgeOffset())[0]);
	edgeWeight = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		sizeof(cl_float) * graphObject.getNumOfEdges(), &(graphObject.getEdgeWeight())[0]);
	
	{
		std::vector<int> zeros(graphObject.getNumOfNodes(), 0);

		// Allocates force buffers shared by CL objects
		fx[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
		fx[1] = cl::Buffer(clContext.getContext(), 
			CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * graphObject.getNumOfNodes(), &zeros[0]);

		fy[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
		fy[1] = cl::Buffer(clContext.getContext(), 
			CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * graphObject.getNumOfNodes(), &zeros[0]);

		fz[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
		fz[1] = cl::Buffer(clContext.getContext(), 
			CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * graphObject.getNumOfNodes(), &zeros[0]);
	}

	// Allocates buffers for partial graph center of mass coordinates
	tmpX[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	tmpX[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

	tmpY[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	tmpY[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

	tmpZ[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	tmpZ[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

	// Initialize GL shared buffers
	glNodeX = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphNode.getOffsetX());
	glNodeY = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphNode.getOffsetY());
	glNodeZ = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphNode.getOffsetZ());

	glNodeScale = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphNode.getScale());

	glEdgeSourceId = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphEdge.getSourceId());

	glEdgeSourceX = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphEdge.getSourceX());
	glEdgeSourceY = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphEdge.getSourceY());
	glEdgeSourceZ = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphEdge.getSourceZ());

	glEdgeTargetId = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphEdge.getTargetId());

	glEdgeTargetX = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphEdge.getTargetX());
	glEdgeTargetY = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphEdge.getTargetY());
	glEdgeTargetZ = cl::BufferGL(clContext.getContext(), CL_MEM_READ_WRITE, glGraphEdge.getTargetZ());

	// Add GL buffers to vector
	sharedBuffers.push_back(glNodeX);
	sharedBuffers.push_back(glNodeY);
	sharedBuffers.push_back(glNodeZ);
	sharedBuffers.push_back(glNodeScale);
	sharedBuffers.push_back(glEdgeSourceId);
	sharedBuffers.push_back(glEdgeSourceX);
	sharedBuffers.push_back(glEdgeSourceY);
	sharedBuffers.push_back(glEdgeSourceZ);
	sharedBuffers.push_back(glEdgeTargetId);
	sharedBuffers.push_back(glEdgeTargetX);
	sharedBuffers.push_back(glEdgeTargetY);
	sharedBuffers.push_back(glEdgeTargetZ);

	// Initializes CL kernels
	clGlobalSwing.init();
	clGlobalTraction.init();
	clGraphCenter.init();
	clSum.init();
	clForceAttr.init();
	clForceRepl.init();
	clUpdateNode.init();
	clUpdateEdge.init();

	// Sets CL kernels initial arguments
	setCLGlobalSwingArgs(true);
	setCLGlobalTractionArgs(true);
	setCLGraphCenterArgs(true);
	setCLForceAttrArgs(true);
	setCLForceReplArgs(true);
	setCLUpdateNodeArgs(true);
	setCLUpdateEdgeArgs(true);
}

void ForceAtlas2Sim::run()
{
	// Acquires all GL buffers before running kernels
	clQueue.acquireGLBuffers(sharedBuffers);

	// Calculates graph center of mass
	if (fa2Params.getFg() || fa2Params.getFsg())
	{
		setCLGraphCenterArgs(false);
		clQueue.runKernel(clGraphCenter);
		sum(graphObject.getNumOfNodes(), centerOfMass);
	}

	// Runs n-body
	setCLForceAttrArgs(false);
	clQueue.runKernel(clForceAttr);

	setCLForceReplArgs(false);
	clQueue.runKernel(clForceRepl);

	// Calculates global swing
	setCLGlobalSwingArgs(false);
	clQueue.runKernel(clGlobalSwing);
	sum(graphObject.getNumOfNodes(), globalSwing);

	// Calculates global traction
	setCLGlobalTractionArgs(false);
	clQueue.runKernel(clGlobalTraction);
	sum(graphObject.getNumOfNodes(), globalTraction);

	// Updates graph
	setCLUpdateNodeArgs(false);
	clQueue.runKernel(clUpdateNode);

	setCLUpdateEdgeArgs(false);
	clQueue.runKernel(clUpdateEdge);

	// Switches force buffers
	forceFront = forceFront == 1 ? 0 : 1;

	// Releases all GL buffers after running kernels
	clQueue.releaseGLBuffers(sharedBuffers);
}
