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
	clInitCenter(clContext.getDevice(), clContext.getContext()),
	clCalcCenter(clContext.getDevice(), clContext.getContext()),
	clNbody(clContext.getDevice(), clContext.getContext()),
	clUpdateNode(clContext.getDevice(), clContext.getContext(), fa2Params_.getFg(), fa2Params_.getFsg()),
	clUpdateEdge(clContext.getDevice(), clContext.getContext()),
	clContext()
{
	cFront = 0;
}

ForceAtlas2Sim::~ForceAtlas2Sim()
{
}

void ForceAtlas2Sim::setCLInitCenterArgs()
{
	clInitCenter.setWorkSize(graphObject.getNumOfNodes());
	clInitCenter.setArg(0, graphObject.getNumOfNodes());
	clInitCenter.setArg(1, glGraphNode.getOffsetX(), CL_MEM_READ_WRITE);
	clInitCenter.setArg(2, glGraphNode.getOffsetY(), CL_MEM_READ_WRITE);
	clInitCenter.setArg(3, glGraphNode.getOffsetZ(), CL_MEM_READ_WRITE);
	clInitCenter.setArg(4, glGraphNode.getScale(), CL_MEM_READ_ONLY);
	clInitCenter.setArg(5, cx[cFront]);
	clInitCenter.setArg(6, cy[cFront]);
	clInitCenter.setArg(7, cz[cFront]);
}

void ForceAtlas2Sim::setCLCalcCenterArgs(unsigned int n, unsigned int workGroupSize)
{
	float* temp = nullptr;

	clCalcCenter.setWorkSize(n);
	clCalcCenter.setArg(0, n);
	clCalcCenter.setArg(1, cx[cFront]);
	clCalcCenter.setArg(2, cy[cFront]);
	clCalcCenter.setArg(3, cz[cFront]);
	clCalcCenter.setArg(4, cx[cFront == 1 ? 0 : 1]);
	clCalcCenter.setArg(5, cy[cFront == 1 ? 0 : 1]);
	clCalcCenter.setArg(6, cz[cFront == 1 ? 0 : 1]);
	clCalcCenter.setArg(7, workGroupSize, temp);
	clCalcCenter.setArg(8, workGroupSize, temp);
	clCalcCenter.setArg(9, workGroupSize, temp);
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
	clUpdateNode.setArg(10, cx[cFront]);
	clUpdateNode.setArg(11, cy[cFront]);
	clUpdateNode.setArg(12, cz[cFront]);
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

void ForceAtlas2Sim::init()
{
	// Allocates force buffers shared by CL objects
	fx = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	fy = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	fz = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

	// Allocates buffers for partial graph center of mass coordinates
	if (fa2Params.getFg() || fa2Params.getFsg())
	{
		cx[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
		cx[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

		cy[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
		cy[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

		cz[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
		cz[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	}

	// Initializes CL kernels
	clInitCenter.init();
	clCalcCenter.init();
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
		setCLInitCenterArgs();
		clInitCenter.run();

		unsigned int n = graphObject.getNumOfNodes();
		unsigned int workGroupSize = clCalcCenter.getMaxWorkGroupSize();

		setCLCalcCenterArgs(n, workGroupSize);
		clCalcCenter.run();

		// Sums partial graph center of mass coordinates
		while (n > workGroupSize)
		{
			n = (unsigned int)std::ceil(n / workGroupSize);
			cFront = cFront == 1 ? 0 : 1;

			setCLCalcCenterArgs(n, workGroupSize);
			clCalcCenter.run();
		}

		setCLUpdateNodeArgsFg();
	}

	clNbody.run();
	clUpdateNode.run();
	clUpdateEdge.run();
}
