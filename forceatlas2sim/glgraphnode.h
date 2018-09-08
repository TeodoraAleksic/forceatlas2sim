#ifndef _GLGRAPHNODE_H_
#define _GLGRAPHNODE_H_

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

class GLGraphNode: public GLBase
{
private:

	static const int lats, longs, numOfIndices;

	const Camera& camera;
	const GraphObject& graphObject;

	unsigned int selectedNode;

	bool isInited;

	unsigned int vao;
	unsigned int vboVertex, vboIndex, vboOffsetX, vboOffsetY, vboOffsetZ, vboScale;
	
	unsigned int program;
	unsigned int uniformProjection, uniformView, uniformModel, uniformNormalMatrix, uniformSelectedNode, uniformCameraPos;

	void initNode(std::vector<float>* vertices, std::vector<unsigned int>* indices);

public:

	GLGraphNode(const Camera& camera_, const GraphObject& graphObject_);
	~GLGraphNode();

	void init();
	void draw();
	void cleanup();

	void setSelectedNode(unsigned int selectedNode_);

	unsigned int getNumOfNodes() const;
	unsigned int getVertices() const;
	unsigned int getIndices() const;
	unsigned int getOffsetX() const;
	unsigned int getOffsetY() const;
	unsigned int getOffsetZ() const;
	unsigned int getScale() const;

};

#endif
