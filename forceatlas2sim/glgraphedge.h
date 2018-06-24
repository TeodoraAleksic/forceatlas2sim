#ifndef _GLGRAPHEDGE_H_
#define _GLGRAPHEDGE_H_

#include "camera.h"
#include "globject.h"
#include "graphobject.h"

class GLGraphEdge : public GLObject
{
private:

	const Camera& camera;
	const GraphObject& graphObject;

	bool isInited;

	unsigned int vao;
	unsigned int vboVertex, vboIndex, vboSourceX, vboSourceY, vboSourceZ, vboTargetX, vboTargetY, vboTargetZ;

	unsigned int program;
	unsigned int uniformProjection, uniformView, uniformModel;

public:

	GLGraphEdge(const Camera& camera_, const GraphObject& graphObject_);
	~GLGraphEdge();

	void init();
	void draw();
	void cleanup();

	unsigned int getNumOfEdges() const;
	unsigned int getSourceX() const;
	unsigned int getSourceY() const;
	unsigned int getSourceZ() const;
	unsigned int getTargetX() const;
	unsigned int getTargetY() const;
	unsigned int getTargetZ() const;

};

#endif

