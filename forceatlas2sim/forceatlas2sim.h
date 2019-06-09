#ifndef _FORCEATLAS2SIM_H_
#define _FORCEATLAS2SIM_H_

#include "clcontext.h"
#include "clglobalswing.h"
#include "clglobaltraction.h"
#include "clforceattr.h"
#include "clforcerepl.h"
#include "clgraphcenter.h"
#include "clqueue.h"
#include "clsum.h"
#include "clupdateedge.h"
#include "clupdatenode.h"
#include "forceatlas2params.h"
#include "glgraphedge.h"
#include "glgraphnode.h"
#include "graphobject.h"

/**
* ForceAtlas2 simulation
*
* Initializes all simulation objects.
*/
class ForceAtlas2Sim
{
private:

	// ForceAtlas2 parameters
	const ForceAtlas2Params& fa2Params;
	// Object containing graph data
	const GraphObject& graphObject;
	// OpenGL graph nodes
	const GLGraphNode& glGraphNode;
	// OpenGL graph edges
	const GLGraphEdge& glGraphEdge;

	// OpenCL context
	CLContext clContext;
	// OpenCL command queue
	CLQueue clQueue;

	// Kernel for calculating the global graph swing
	CLGlobalSwing clGlobalSwing;
	// Kernel for calculating the global graph traction
	CLGlobalTraction clGlobalTraction;
	// Kernel for calculating the center of the graph
	CLGraphCenter clGraphCenter;
	// Kernel for calculating a generic sum
	CLSum clSum;
	// Kernel for calculating the force of attraction
	CLForceAttr clForceAttr;
	// Kernel for calculating the force of repulsion
	CLForceRepl clForceRepl;
	// Kernel for updating the graph's nodes
	CLUpdateNode clUpdateNode;
	// Kernel for updating the graph's edges
	CLUpdateEdge clUpdateEdge;

	// Buffer for the graph's center of mass
	cl::Buffer centerOfMass;
	// Buffer for the graph's global swing
	cl::Buffer globalSwing;
	// Buffer for the graph's global traction
	cl::Buffer globalTraction;

	// Buffer for the offsets of nodes in the edge source node array
	cl::Buffer sourceNodeOffset;
	// Buffer for the edge weights
	cl::Buffer edgeWeight;

	// Id of the currently used force buffer
	int forceFront;
	// Switchable force buffers for the x axis
	cl::Buffer fx[2];
	// Switchable force buffers for the y axis
	cl::Buffer fy[2];
	// Switchable force buffers for the z axis
	cl::Buffer fz[2];

	// Id of the currently used temporary buffer
	int tmpFront;
	// Switchable temporary buffers for the x axis
	cl::Buffer tmpX[2];
	// Switchable temporary buffers for the y axis
	cl::Buffer tmpY[2];
	// Switchable temporary buffers for the z axis
	cl::Buffer tmpZ[2];

	// Array of buffers shared between OpenCL and OpenGL
	std::vector<cl::Memory> sharedBuffers;

	// Shared buffer for the node x axes
	cl::BufferGL glNodeX;
	// Shared buffer for the node y axes
	cl::BufferGL glNodeY;
	// Shared buffer for the node z axes
	cl::BufferGL glNodeZ;
	// Shared buffer for the node degrees
	cl::BufferGL glNodeScale;
	// Shared buffer for the edge source node Ids
	cl::BufferGL glEdgeSourceId;
	// Shared buffer for the edge source node x axes
	cl::BufferGL glEdgeSourceX;
	// Shared buffer for the edge source node y axes
	cl::BufferGL glEdgeSourceY;
	// Shared buffer for the edge source node z axes
	cl::BufferGL glEdgeSourceZ;
	// Shared buffer for the edge target node Ids
	cl::BufferGL glEdgeTargetId;
	// Shared buffer for the edge target node x axes
	cl::BufferGL glEdgeTargetX;
	// Shared buffer for the edge target node y axes
	cl::BufferGL glEdgeTargetY;
	// Shared buffer for the edge target node z axes
	cl::BufferGL glEdgeTargetZ;

	/**
	* Sets the arguments for the global graph swing kernel
	*
	* @param init Indicates if the kernel arguments are being set for the first time
	*/
	void setCLGlobalSwingArgs(bool init);

	/**
	* Sets the arguments for the global graph traction kernel
	*
	* @param init Indicates if the kernel arguments are being set for the first time
	*/
	void setCLGlobalTractionArgs(bool init);

	/**
	* Sets the arguments for the graph center swing kernel
	*
	* @param init Indicates if the kernel arguments are being set for the first time
	*/
	void setCLGraphCenterArgs(bool init);

	/**
	* Sets the arguments for the force of attraction kernel
	*
	* @param init Indicates if the kernel arguments are being set for the first time
	*/
	void setCLForceAttrArgs(bool init);

	/**
	* Sets the arguments for the force of repulsion kernel
	*
	* @param init Indicates if the kernel arguments are being set for the first time
	*/
	void setCLForceReplArgs(bool init);

	/**
	* Sets the arguments for the update nodes kernel
	*
	* @param init Indicates if the kernel arguments are being set for the first time
	*/
	void setCLUpdateNodeArgs(bool init);

	/**
	* Sets the arguments for the update edges kernel
	*
	* @param init Indicates if the kernel arguments are being set for the first time
	*/
	void setCLUpdateEdgeArgs(bool init);

	/**
	* Sets the arguments for the generic sum kernel
	*
	* @param n				Size of the buffer whose sum is calculated
	* @param workGroupSize	Size of the workgroup
	* @param global			Buffer containing the simulation's global variables
	*/
	void setCLSumArgs(unsigned int n, unsigned int workGroupSize, cl::Buffer global);

	/**
	* Calculates the sum of elements stored in the current temporary buffer.
	* Writes the result in the buffer containing the simulation's global variables.
	*
	* @param n		Size of the buffer whose sum is calculated
	* @param global	Buffer containing the simulation's global variables
	*/
	void sum(unsigned int n, cl::Buffer global);

public:

	ForceAtlas2Sim(
		const ForceAtlas2Params& fa2Params_, 
		const GraphObject& graphObject_, 
		const GLGraphNode& glGraphNode_, 
		const GLGraphEdge& glGraphEdge_);
	~ForceAtlas2Sim();

	// Initializes all simulation objects
	void init();
	// Runs the simulation
	void run();

};

#endif
