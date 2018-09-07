#ifndef _GLGRAPHEDGE_H_
#define _GLGRAPHEDGE_H_

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

class GLGraphEdge : public GLBase
{
private:

	const Camera& camera;
	const GraphObject& graphObject;

	unsigned int selectedNode;

	bool isInited;

	unsigned int vao;
	unsigned int vboVertex, vboIndex;
	unsigned int vboSourceId, vboTargetId;
	unsigned int vboSourceX, vboSourceY, vboSourceZ, vboTargetX, vboTargetY, vboTargetZ;

	unsigned int program;
	unsigned int uniformProjection, uniformView, uniformModel, uniformSelectedNode;

public:

	GLGraphEdge(const Camera& camera_, const GraphObject& graphObject_);
	~GLGraphEdge();

	void init();
	void draw();
	void cleanup();

	void setSelectedNode(unsigned int selectedNode_);

	unsigned int getNumOfEdges() const;
	unsigned int getSourceId() const;
	unsigned int getTargetId() const;
	unsigned int getSourceX() const;
	unsigned int getSourceY() const;
	unsigned int getSourceZ() const;
	unsigned int getTargetX() const;
	unsigned int getTargetY() const;
	unsigned int getTargetZ() const;

};

#endif

