#include "forceatlas2sim.h"

ForceAtlas2Sim::ForceAtlas2Sim():
	clNbody(clContext.getDevice(), clContext.getContext()),
	clUpdateNode(clContext.getDevice(), clContext.getContext()),
	clUpdateEdge(clContext.getDevice(), clContext.getContext()),
	clContext()
{
}

ForceAtlas2Sim::~ForceAtlas2Sim()
{
}

void ForceAtlas2Sim::init(const ForceAtlas2Params& fa2Params, const GraphObject& graphObject, const GLGraphNode& glGraphNode, const GLGraphEdge& glGraphEdge)
{
	// Allocates force buffers shared by CL objects
	fx = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	fy = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());
	fz = cl::Buffer(clContext.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * graphObject.getNumOfNodes());

	// Initializes CL objects
	clNbody.init();
	clUpdateNode.init();
	clUpdateEdge.init();

	// Sets arguments for n-body
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

	// Sets arguments for update node
	clUpdateNode.setWorkSize(graphObject.getNumOfNodes());
	clUpdateNode.setArg(0, graphObject.getNumOfNodes());
	clUpdateNode.setArg(1, glGraphNode.getOffsetX(), CL_MEM_READ_WRITE);
	clUpdateNode.setArg(2, glGraphNode.getOffsetY(), CL_MEM_READ_WRITE);
	clUpdateNode.setArg(3, glGraphNode.getOffsetZ(), CL_MEM_READ_WRITE);
	clUpdateNode.setArg(4, glGraphNode.getScale(), CL_MEM_READ_ONLY);
	clUpdateNode.setArg(5, fx);
	clUpdateNode.setArg(6, fy);
	clUpdateNode.setArg(7, fz);

	// Sets arguments for update edge
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

void ForceAtlas2Sim::run()
{
	clNbody.run();
	clUpdateNode.run();
	clUpdateEdge.run();
}
