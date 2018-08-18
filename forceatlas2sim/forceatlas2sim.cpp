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
	forceFront = 0;
	tmpFront = 0;
}

ForceAtlas2Sim::~ForceAtlas2Sim()
{
}

void ForceAtlas2Sim::setCLGlobalSwingArgs()
{
	clGlobalSwing.setWorkSize(graphObject.getNumOfNodes());
	clGlobalSwing.setArg(0, graphObject.getNumOfNodes());
	clGlobalSwing.setArg(1, tmpX[tmpFront]);
	clGlobalSwing.setArg(2, tmpY[tmpFront]);
	clGlobalSwing.setArg(3, tmpZ[tmpFront]);
	clGlobalSwing.setArg(4, fx[forceFront]);
	clGlobalSwing.setArg(5, fy[forceFront]);
	clGlobalSwing.setArg(6, fz[forceFront]);
	clGlobalSwing.setArg(7, fx[forceFront == 1 ? 0 : 1]);
	clGlobalSwing.setArg(8, fy[forceFront == 1 ? 0 : 1]);
	clGlobalSwing.setArg(9, fz[forceFront == 1 ? 0 : 1]);
	clGlobalSwing.setArg(10, glGraphNode.getScale(), CL_MEM_READ_ONLY);
}

void ForceAtlas2Sim::setCLGlobalTractionArgs()
{
	clGlobalTraction.setWorkSize(graphObject.getNumOfNodes());
	clGlobalTraction.setArg(0, graphObject.getNumOfNodes());
	clGlobalTraction.setArg(1, tmpX[tmpFront]);
	clGlobalTraction.setArg(2, tmpY[tmpFront]);
	clGlobalTraction.setArg(3, tmpZ[tmpFront]);
	clGlobalTraction.setArg(4, fx[forceFront]);
	clGlobalTraction.setArg(5, fy[forceFront]);
	clGlobalTraction.setArg(6, fz[forceFront]);
	clGlobalTraction.setArg(7, fx[forceFront == 1 ? 0 : 1]);
	clGlobalTraction.setArg(8, fy[forceFront == 1 ? 0 : 1]);
	clGlobalTraction.setArg(9, fz[forceFront == 1 ? 0 : 1]);
	clGlobalTraction.setArg(10, glGraphNode.getScale(), CL_MEM_READ_ONLY);
}

void ForceAtlas2Sim::setCLGraphCenterArgs()
{
	clGraphCenter.setWorkSize(graphObject.getNumOfNodes());
	clGraphCenter.setArg(0, graphObject.getNumOfNodes());
	clGraphCenter.setArg(1, glGraphNode.getOffsetX(), CL_MEM_READ_WRITE);
	clGraphCenter.setArg(2, glGraphNode.getOffsetY(), CL_MEM_READ_WRITE);
	clGraphCenter.setArg(3, glGraphNode.getOffsetZ(), CL_MEM_READ_WRITE);
	clGraphCenter.setArg(4, glGraphNode.getScale(), CL_MEM_READ_ONLY);
	clGraphCenter.setArg(5, tmpX[tmpFront]);
	clGraphCenter.setArg(6, tmpY[tmpFront]);
	clGraphCenter.setArg(7, tmpZ[tmpFront]);
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
	clNbody.setArg(7, fx[forceFront]);
	clNbody.setArg(8, fy[forceFront]);
	clNbody.setArg(9, fz[forceFront]);
}

void ForceAtlas2Sim::setCLUpdateNodeArgs()
{
	clUpdateNode.setWorkSize(graphObject.getNumOfNodes());
	clUpdateNode.setArg(0, graphObject.getNumOfNodes());
	clUpdateNode.setArg(1, glGraphNode.getOffsetX(), CL_MEM_READ_WRITE);
	clUpdateNode.setArg(2, glGraphNode.getOffsetY(), CL_MEM_READ_WRITE);
	clUpdateNode.setArg(3, glGraphNode.getOffsetZ(), CL_MEM_READ_WRITE);
	clUpdateNode.setArg(4, glGraphNode.getScale(), CL_MEM_READ_ONLY);
	clUpdateNode.setArg(5, fx[forceFront]);
	clUpdateNode.setArg(6, fy[forceFront]);
	clUpdateNode.setArg(7, fz[forceFront]);
}

void ForceAtlas2Sim::setCLUpdateNodeArgsFg()
{
	clUpdateNode.setArg(8, fa2Params.getKg());
	clUpdateNode.setArg(9, graphObject.getTotalDegree());
	clUpdateNode.setArg(10, centerOfMass);
}

void ForceAtlas2Sim::setCLUpdateEdgeArgs()
{
	clUpdateEdge.setWorkSize(graphObject.getNumOfEdges());
	clUpdateEdge.setArg(0, graphObject.getNumOfEdges());
	clUpdateEdge.setArg(1, glGraphNode.getOffsetX(), CL_MEM_READ_ONLY);
	clUpdateEdge.setArg(2, glGraphNode.getOffsetY(), CL_MEM_READ_ONLY);
	clUpdateEdge.setArg(3, glGraphNode.getOffsetZ(), CL_MEM_READ_ONLY);
	clUpdateEdge.setArg(4, sizeof(cl_uint) * graphObject.getNumOfEdges(), 
		&(graphObject.getSourceId())[0], CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);
	clUpdateEdge.setArg(5, glGraphEdge.getSourceX(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(6, glGraphEdge.getSourceY(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(7, glGraphEdge.getSourceZ(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(8, sizeof(cl_uint) * graphObject.getNumOfEdges(), 
		&(graphObject.getTargetId())[0], CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR);
	clUpdateEdge.setArg(9, glGraphEdge.getTargetX(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(10, glGraphEdge.getTargetY(), CL_MEM_READ_WRITE);
	clUpdateEdge.setArg(11, glGraphEdge.getTargetZ(), CL_MEM_READ_WRITE);
}

void ForceAtlas2Sim::sum(unsigned int n, cl::Buffer graphGlobal)
{
	unsigned int workGroupSize = clSum.getMaxWorkGroupSize();

	setCLSumArgs(n, workGroupSize, graphGlobal);
	clSum.run();
	tmpFront = tmpFront == 1 ? 0 : 1;

	// Sums partial sums
	while (n > workGroupSize)
	{
		n = (unsigned int)std::ceil(n / workGroupSize);
		setCLSumArgs(n, workGroupSize, graphGlobal);
		clSum.run();
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

	// Initializes CL kernels
	clGlobalSwing.init();
	clGlobalTraction.init();
	clGraphCenter.init();
	clSum.init();
	clNbody.init();
	clUpdateNode.init();
	clUpdateEdge.init();
}

void ForceAtlas2Sim::run()
{
	// Calculates graph center of mass
	if (fa2Params.getFg() || fa2Params.getFsg())
	{
		setCLGraphCenterArgs();
		clGraphCenter.run();

		sum(graphObject.getNumOfNodes(), centerOfMass);

		setCLUpdateNodeArgsFg();
	}

	// Runs n-body
	setCLNbodyArgs();
	clNbody.run();

	setCLGlobalSwingArgs();
	clGlobalSwing.run();

	setCLGlobalTractionArgs();
	clGlobalTraction.run();

	// Updates graph
	setCLUpdateNodeArgs();
	clUpdateNode.run();

	setCLUpdateEdgeArgs();
	clUpdateEdge.run();

	// Switches force buffers
	forceFront = forceFront == 1 ? 0 : 1;
}
