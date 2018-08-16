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
	clNbody(clContext.getDevice(), clContext.getContext()),
	clUpdateNode(clContext.getDevice(), clContext.getContext(), fa2Params_.getFg(), fa2Params_.getFsg()),
	clUpdateEdge(clContext.getDevice(), clContext.getContext()),
	clContext()
{
	front = 0;
}

ForceAtlas2Sim::~ForceAtlas2Sim()
{
}

void ForceAtlas2Sim::setCLGlobalSwing()
{
}

void ForceAtlas2Sim::setCLGlobalTraction()
{
}

void ForceAtlas2Sim::setCLGraphCenterArgs()
{
	clGraphCenter.setWorkSize(graphObject.getNumOfNodes());
	clGraphCenter.setArg(0, graphObject.getNumOfNodes());
	clGraphCenter.setArg(1, glGraphNode.getOffsetX(), CL_MEM_READ_WRITE);
	clGraphCenter.setArg(2, glGraphNode.getOffsetY(), CL_MEM_READ_WRITE);
	clGraphCenter.setArg(3, glGraphNode.getOffsetZ(), CL_MEM_READ_WRITE);
	clGraphCenter.setArg(4, glGraphNode.getScale(), CL_MEM_READ_ONLY);
	clGraphCenter.setArg(5, bufferX[front]);
	clGraphCenter.setArg(6, bufferY[front]);
	clGraphCenter.setArg(7, bufferZ[front]);
}

void ForceAtlas2Sim::setCLSumArgs(unsigned int n, unsigned int workGroupSize)
{
	float* temp = nullptr;

	clSum.setWorkSize(n);
	clSum.setArg(0, n);
	clSum.setArg(1, bufferX[front]);
	clSum.setArg(2, bufferY[front]);
	clSum.setArg(3, bufferZ[front]);
	clSum.setArg(4, bufferX[front == 1 ? 0 : 1]);
	clSum.setArg(5, bufferY[front == 1 ? 0 : 1]);
	clSum.setArg(6, bufferZ[front == 1 ? 0 : 1]);
	clSum.setArg(7, workGroupSize, temp);
	clSum.setArg(8, workGroupSize, temp);
	clSum.setArg(9, workGroupSize, temp);
}

void ForceAtlas2Sim::setCLNbodyArgs()
{
	clNbody.setWorkSize(graphObject.getNumOfNodes());
	clNbody.setArg(0, graphObject.getNumOfNodes());
	clNbody.setArg(1, fa2Params.getKr());
	clNbody.setArg(2, fa2Params.getKrp());
	clNbody.setArg(3, glGraphNode.getOffsetX(), CL_MEM_READ_ONLY);
	clNbody.setArg(4, glGraphNode.getOffsetY(), CL_MEM_READ_ONLY);
	clNbody.setArg(5, glGraphNode.getOffsetZ(), CL_MEM_READ_ONLY);
	clNbody.setArg(6, glGraphNode.getScale(), CL_MEM_READ_ONLY);
	clNbody.setArg(7, fx);
	clNbody.setArg(8, fy);
	clNbody.setArg(9, fz);
}

void ForceAtlas2Sim::setCLUpdateNodeArgs()
{
	clUpdateNode.setWorkSize(graphObject.getNumOfNodes());
	clUpdateNode.setArg(0, graphObject.getNumOfNodes());
	clUpdateNode.setArg(1, glGraphNode.getOffsetX(), CL_MEM_READ_WRITE);
	clUpdateNode.setArg(2, glGraphNode.getOffsetY(), CL_MEM_READ_WRITE);
	clUpdateNode.setArg(3, glGraphNode.getOffsetZ(), CL_MEM_READ_WRITE);
	clUpdateNode.setArg(4, glGraphNode.getScale(), CL_MEM_READ_ONLY);
	clUpdateNode.setArg(5, fx);
	clUpdateNode.setArg(6, fy);
	clUpdateNode.setArg(7, fz);
}

void ForceAtlas2Sim::setCLUpdateNodeArgsFg()
{
	clUpdateNode.setArg(8, fa2Params.getKg());
	clUpdateNode.setArg(9, graphObject.getTotalDegree());
	clUpdateNode.setArg(10, bufferX[front]);
	clUpdateNode.setArg(11, bufferY[front]);
	clUpdateNode.setArg(12, bufferZ[front]);
}

void ForceAtlas2Sim::setCLUpdateEdgeArgs()
{
	clUpdateEdge.setWorkSize(graphObject.getNumOfEdges());
	clUpdateEdge.setArg(0, graphObject.getNumOfEdges());
	clUpdateEdge.setArg(1, glGraphNode.getOffsetX(), CL_MEM_READ_ONLY);
	clUpdateEdge.setArg(2, glGraphNode.getOffsetY(), CL_MEM_READ_ONLY);
	clUpdateEdge.setArg(3, glGraphNode.getOffsetZ(), CL_MEM_READ_ONLY);
	clUpdateEdge.setArg(4, sizeof(cl_uint) * graphObject.getNumOfEdges(), &(graphObject.getSourceId())[0], CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);
	clUpdateEdge.setArg(5, glGraphEdge.getSourceX(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(6, glGraphEdge.getSourceY(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(7, glGraphEdge.getSourceZ(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(8, sizeof(cl_uint) * graphObject.getNumOfEdges(), &(graphObject.getTargetId())[0], CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);
	clUpdateEdge.setArg(9, glGraphEdge.getTargetX(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(10, glGraphEdge.getTargetY(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(11, glGraphEdge.getTargetZ(), CL_MEM_READ_WRITE);
}

void ForceAtlas2Sim::sum(unsigned int n)
{
	unsigned int workGroupSize = clSum.getMaxWorkGroupSize();

	setCLSumArgs(n, workGroupSize);
	clSum.run();

	// Sums partial sums
	while (n > workGroupSize)
	{
		n = (unsigned int)std::ceil(n / workGroupSize);
		front = front == 1 ? 0 : 1;

		setCLSumArgs(n, workGroupSize);
		clSum.run();
	}
}

void ForceAtlas2Sim::init()
{
	// Allocates force buffers shared by CL objects
	fx = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	fy = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	fz = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

	// Allocates buffers for partial graph center of mass coordinates
	if (fa2Params.getFg() || fa2Params.getFsg())
	{
		bufferX[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
		bufferX[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

		bufferY[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
		bufferY[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

		bufferZ[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
		bufferZ[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	}

	// Initializes CL kernels
	clGraphCenter.init();
	clSum.init();
	clNbody.init();
	clUpdateNode.init();
	clUpdateEdge.init();

	// Sets CL kernel arguments
	setCLNbodyArgs();
	setCLUpdateNodeArgs();
	setCLUpdateEdgeArgs();
}

void ForceAtlas2Sim::run()
{
	// Calculates graph center of mass
	if (fa2Params.getFg() || fa2Params.getFsg())
	{
		// Calculates partial graph center of mass coordinates
		setCLGraphCenterArgs();
		clGraphCenter.run();

		sum(graphObject.getNumOfNodes());

		setCLUpdateNodeArgsFg();
	}

	clNbody.run();
	clUpdateNode.run();
	clUpdateEdge.run();
}
