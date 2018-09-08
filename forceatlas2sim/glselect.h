#ifndef _GLSELECT_H_
#define _GLSELECT_H_

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

class GLSelect : public GLBase
{
private:

	static const int lats, longs, numOfIndices;

	const Camera& camera;
	const GraphObject& graphObject;

	bool isInited;

	unsigned int vao;
	unsigned int vboVertex, vboIndex, vboOffsetX, vboOffsetY, vboOffsetZ, vboScale;

	unsigned int program;
	unsigned int uniformProjection, uniformView, uniformModel, uniformNormalMatrix, uniformCameraPos;

public:

	GLSelect(const Camera& camera_, const GraphObject& graphObject_);
	~GLSelect();

	void init();
	void draw();
	void cleanup();

	void setVboVertex(unsigned int vboVertex_);
	void setVboIndex(unsigned int vboIndex_);
	void setVboOffsetX(unsigned int vboOffsetX_);
	void setVboOffsetY(unsigned int vboOffsetY_);
	void setVboOffsetZ(unsigned int vboOffsetZ_);
	void setVboScale(unsigned int vboScale_);

};

#endif
