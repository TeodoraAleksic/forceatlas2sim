#ifndef _FORCEATLAS2SIM_H_
#define _FORCEATLAS2SIM_H_

#include "clcontext.h"
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

	CLContext clContext;

	CLNbody clNbody;
	CLUpdateNode clUpdateNode;
	CLUpdateEdge clUpdateEdge;

	cl::Buffer fx, fy, fz;

public:

	ForceAtlas2Sim();
	~ForceAtlas2Sim();

	void init(const ForceAtlas2Params& fa2Params, const GraphObject& graphObject, const GLGraphNode& glGraphNode, const GLGraphEdge& glGraphEdge);
	void run();

};

#endif
