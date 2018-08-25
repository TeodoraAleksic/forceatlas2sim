#ifndef _FORCEATLAS2SIM_H_
#define _FORCEATLAS2SIM_H_

#include "clcontext.h"
#include "clglobalswing.h"
#include "clglobaltraction.h"
#include "clforceattr.h"
#include "clforcerepl.h"
#include "clgraphcenter.h"
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

	CLGlobalSwing clGlobalSwing;
	CLGlobalTraction clGlobalTraction;
	CLGraphCenter clGraphCenter;
	CLSum clSum;
	CLForceAttr clForceAttr;
	CLForceRepl clForceRepl;
	CLUpdateNode clUpdateNode;
	CLUpdateEdge clUpdateEdge;

	cl::Buffer centerOfMass, globalSwing, globalTraction;

	cl::Buffer sourceId, targetId, edgeOffset, edgeWeight;

	int forceFront;
	cl::Buffer fx[2], fy[2], fz[2];

	int tmpFront;
	cl::Buffer tmpX[2], tmpY[2], tmpZ[2];

	void setCLGlobalSwingArgs();
	void setCLGlobalTractionArgs();
	void setCLGraphCenterArgs();
	void setCLForceAttr();
	void setCLForceRepl();
	void setCLUpdateNodeArgs();
	void setCLUpdateEdgeArgs();

	void setCLSumArgs(unsigned int n, unsigned int workGroupSize, cl::Buffer global);

	void sum(unsigned int n, cl::Buffer global);

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
