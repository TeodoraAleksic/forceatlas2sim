#ifndef _GLGRAPHEDGE_H_
#define _GLGRAPHEDGE_H_

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

/**
* OpenGL graph edge
*
* Builds OpenGL program for drawing the graph's edges.
* Copies the edge data into buffers on the GPU. 
* Draws the edges.
*/
class GLGraphEdge : public GLBase
{
private:

	// Camera of the simulation
	const Camera& camera;
	// Object containing graph data
	const GraphObject& graphObject;

	// The Id of node currently selected in the simulation
	unsigned int selectedNode;

	// Indicates if the graph's edges have already been initialized
	bool isInited;

	// Id of the VAO for all the nodes' VBOs
	unsigned int vao;

	// Id of the VBO for the sphere's vertices
	unsigned int vboVertex;
	// Id of the VBO for the sphere's indices
	unsigned int vboIndex;
	// Id of the VBO for the source nodes' Ids
	unsigned int vboSourceId;
	// Id of the VBO for the target nodes' Ids
	unsigned int vboTargetId;
	// Id of the VBO for the source nodes' x axis offsets
	unsigned int vboSourceX;
	// Id of the VBO for the source nodes' y axis offsets
	unsigned int vboSourceY;
	// Id of the VBO for the source nodes' z axis offsets
	unsigned int vboSourceZ;
	// Id of the VBO for the target nodes' x axis offsets
	unsigned int vboTargetX;
	// Id of the VBO for the target nodes' y axis offsets
	unsigned int vboTargetY;
	// Id of the VBO for the target nodes' z axis offsets
	unsigned int vboTargetZ;

	// Id of the OpenGL program used for drawing the graph's edges
	unsigned int program;

	// Location of the uniform variable for the projection matrix in the shaders
	unsigned int uniformProjection;
	// Location of the uniform variable for the view matrix in the shaders
	unsigned int uniformView;
	// Location of the uniform variable for the model matrix in the shaders
	unsigned int uniformModel;
	// Location of the uniform variable for the selected node in the shaders
	unsigned int uniformSelectedNode;

public:

	GLGraphEdge(const Camera& camera_, const GraphObject& graphObject_);
	~GLGraphEdge();

	// Initializes the graph's nodes
	void init();
	// Draws the graph's nodes
	void draw();
	// Cleans up the graph's nodes
	void cleanup();

	/**
	* Sets the node currently selected in the simulation
	*
	* @param selectedNode_ Id of the currently selected node
	*/
	void setSelectedNode(unsigned int selectedNode_);

	// Gets the number of edges in the graph
	unsigned int getNumOfEdges() const;
	// Gets the VBO for the source nodes' Ids
	unsigned int getVboSourceId() const;
	// Gets the VBO for the target nodes' Ids
	unsigned int getVboTargetId() const;
	// Gets the VBO for the source nodes' x axis offsets
	unsigned int getVboSourceX() const;
	// Gets the VBO for the source nodes' y axis offsets
	unsigned int getVboSourceY() const;
	// Gets the VBO for the source nodes' z axis offsets
	unsigned int getVboSourceZ() const;
	// Gets the VBO for the target nodes' x axis offsets
	unsigned int getVboTargetX() const;
	// Gets the VBO for the target nodes' y axis offsets
	unsigned int getVboTargetY() const;
	// Gets the VBO for the target nodes' z axis offsets
	unsigned int getVboTargetZ() const;

};

#endif

