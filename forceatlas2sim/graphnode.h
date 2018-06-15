#ifndef _GRAPHNODE_H_
#define _GRAPHNODE_H_

#include "camera.h"
#include "globject.h"
#include "graphobject.h"

class GraphNode: public GLObject
{
private:

	static const int lats, longs, numOfIndices;

	Camera& camera;
	GraphObject& graphObject;

	bool isInited;

	unsigned int vao;
	unsigned int vboVertex, vboIndex, vboOffsetX, vboOffsetY, vboOffsetZ, vboScale;
	
	unsigned int program;
	unsigned int uniformProjection, uniformView, uniformModel;

	void initNode(std::vector<float>* vertices, std::vector<unsigned int>* indices);

public:

	GraphNode(Camera& camera_, GraphObject& graphObject_);
	~GraphNode();

	void init();
	void draw();
	void cleanup();

};

#endif
