#ifndef _GLSELECT_H_
#define _GLSELECT_H_

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

/**
* OpenGL node selection
*
* Implements feature for node selection.
* Draws the entire graph with each node 
* in a different color.
*/
class GLSelect : public GLBase
{
private:

	// The latitude of the sphere representing a graph's node
	static const int lats;
	// The longitude of the sphere representing a graph's node
	static const int longs;
	// The number of indices of the sphere representing a graph's node
	static const int numOfIndices;

	// Camera of the simulation
	const Camera& camera;
	// Object containing graph data
	const GraphObject& graphObject;

	// Indicates if the graph's nodes have already been initialized
	bool isInited;

	// Id of the VAO for all the nodes' VBOs
	unsigned int vao;

	// Id of the VBO for the sphere's vertices
	unsigned int vboVertex;
	// Id of the VBO for the sphere's indices
	unsigned int vboIndex;
	// Id of the VBO for the nodes' x axis offsets 
	unsigned int vboOffsetX;
	// Id of the VBO for the nodes' y axis offsets
	unsigned int vboOffsetY;
	// Id of the VBO for the nodes' z axis offsets
	unsigned int vboOffsetZ;
	// Id of the VBO for the nodes' degrees
	unsigned int vboScale;

	// Id of the OpenGL program used for drawing the graph's nodes
	unsigned int program;

	// Location of the uniform variable for the projection matrix in the shaders
	unsigned int uniformProjection;
	// Location of the uniform variable for the view matrix in the shaders
	unsigned int uniformView;
	// Location of the uniform variable for the model matrix in the shaders
	unsigned int uniformModel;
	// Location of the uniform variable for the normal matrix in the shaders
	unsigned int uniformNormalMatrix;
	// Location of the uniform variable for the camera position in the shaders
	unsigned int uniformCameraPos;

public:

	GLSelect(const Camera& camera_, const GraphObject& graphObject_);
	~GLSelect();

	// Initializes the graph's nodes
	void init();
	// Draws the graph's nodes
	void draw();
	// Cleans up the graph's nodes
	void cleanup();

	// Sets the VBO for the sphere's vertices
	void setVboVertex(unsigned int vboVertex_);
	// Sets the VBO for the sphere's indices
	void setVboIndex(unsigned int vboIndex_);
	// Sets the VBO for the nodes' x axis offsets
	void setVboOffsetX(unsigned int vboOffsetX_);
	// Sets the VBO for the nodes' y axis offsets
	void setVboOffsetY(unsigned int vboOffsetY_);
	// Sets the VBO for the nodes' z axis offsets
	void setVboOffsetZ(unsigned int vboOffsetZ_);
	// Sets the VBO for the nodes' degrees
	void setVboScale(unsigned int vboScale_);

};

#endif
