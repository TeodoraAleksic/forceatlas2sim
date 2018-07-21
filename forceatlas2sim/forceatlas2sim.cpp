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
	clGravity(clContext.getDevice(), clContext.getContext()),
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

void ForceAtlas2Sim::setCLGravityArgs(unsigned int n, unsigned int workGroupSize)
{
	float* temp = nullptr;

	clGravity.setArg(0, n);
	clGravity.setArg(1, cx[cFront]);
	clGravity.setArg(2, cy[cFront]);
	clGravity.setArg(3, cz[cFront]);
	clGravity.setArg(4, cx[cFront == 1 ? 0 : 1]);
	clGravity.setArg(5, cy[cFront == 1 ? 0 : 1]);
	clGravity.setArg(6, cz[cFront == 1 ? 0 : 1]);
	clGravity.setArg(7, workGroupSize, temp);
	clGravity.setArg(8, workGroupSize, temp);
	clGravity.setArg(9, workGroupSize, temp);
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
		std::vector<float> init(graphObject.getNumOfNodes(), 0.0f);

		cx[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
			sizeof(cl_float) * graphObject.getNumOfNodes(), &init[0]);
		cx[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
			sizeof(cl_float) * graphObject.getNumOfNodes(), &init[0]);

		cy[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
			sizeof(cl_float) * graphObject.getNumOfNodes(), &init[0]);
		cy[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
			sizeof(cl_float) * graphObject.getNumOfNodes(), &init[0]);

		cz[0] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
			sizeof(cl_float) * graphObject.getNumOfNodes(), &init[0]);
		cz[1] = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
			sizeof(cl_float) * graphObject.getNumOfNodes(), &init[0]);
	}

	// Initializes CL kernels
	clGravity.init();
	clNbody.init();
	clUpdateNode.init();
	clUpdateEdge.init();

	// Sets CL kernel arguments
	setCLNbodyArgs();
	setCLUpdateNodeArgs();
	setCLUpdateEdgeArgs();

	// Uses gravity force to update node positions
	if (fa2Params.getFg() || fa2Params.getFsg())
		setCLUpdateNodeArgsFg();
}

void ForceAtlas2Sim::run()
{
	clNbody.run();
	clUpdateNode.run();
	clUpdateEdge.run();

	// Calculates graph center of mass
	if (fa2Params.getFg() || fa2Params.getFsg())
	{
		unsigned int n = graphObject.getNumOfNodes();
		unsigned int workGroupSize = clGravity.getMaxWorkGroupSize();

		clGravity.setWorkSize(n);
		setCLGravityArgs(n, workGroupSize);
		clGravity.run();

		// Sums partial graph center of mass coordinates
		while (n > workGroupSize)
		{
			n = (unsigned int)std::ceil(n / workGroupSize);
			cFront = cFront == 1 ? 0 : 1;

			clGravity.setWorkSize(n);
			setCLGravityArgs(n, workGroupSize);
			clGravity.run();
		}

		setCLUpdateNodeArgsFg();
	}
}
