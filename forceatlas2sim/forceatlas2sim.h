#ifndef _FORCEATLAS2SIM_H_
#define _FORCEATLAS2SIM_H_

#include "clcalccenter.h"
#include "clcontext.h"
#include "clinitcenter.h"
#include "clnbody.h"
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

	CLInitCenter clInitCenter;
	CLCalcCenter clCalcCenter;
	CLNbody clNbody;
	CLUpdateNode clUpdateNode;
	CLUpdateEdge clUpdateEdge;

	cl::Buffer fx, fy, fz;

	int cFront;
	cl::Buffer cx[2], cy[2], cz[2];

	void setCLInitCenterArgs();
	void setCLCalcCenterArgs(unsigned int n, unsigned int workGroupSize);
	void setCLNbodyArgs();
	void setCLUpdateNodeArgs();
	void setCLUpdateNodeArgsFg();
	void setCLUpdateEdgeArgs();

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
