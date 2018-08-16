#ifndef _FORCEATLAS2SIM_H_
#define _FORCEATLAS2SIM_H_

#include "clcontext.h"
#include "clgraphcenter.h"
#include "clnbody.h"
#include "clsum.h"
#include "clupdateedge.h"
#include "clupdatenode.h"
#include "forceatlas2params.h"
#include "glgraphedge.h"
#include "glgraphnode.h"
#include "graphobject.h"

class ForceAtlas2Sim
{
private:

	const ForceAtlas2Params& fa2Params;
	const GraphObject& graphObject;
	const GLGraphNode& glGraphNode;
	const GLGraphEdge& glGraphEdge;

	CLContext clContext;

	CLGraphCenter clGraphCenter;
	CLSum clSum;
	CLNbody clNbody;
	CLUpdateNode clUpdateNode;
	CLUpdateEdge clUpdateEdge;

	cl::Buffer fx, fy, fz;

	int front;
	cl::Buffer bufferX[2], bufferY[2], bufferZ[2];

	void setCLGraphCenterArgs();
	void setCLSumArgs(unsigned int n, unsigned int workGroupSize);
	void setCLNbodyArgs();
	void setCLUpdateNodeArgs();
	void setCLUpdateNodeArgsFg();
	void setCLUpdateEdgeArgs();

	void sum(unsigned int n);

public:

	ForceAtlas2Sim(
		const ForceAtlas2Params& fa2Params_, 
		const GraphObject& graphObject_, 
		const GLGraphNode& glGraphNode_, 
		const GLGraphEdge& glGraphEdge_);
	~ForceAtlas2Sim();

	void init();
	void run();

};

#endif
