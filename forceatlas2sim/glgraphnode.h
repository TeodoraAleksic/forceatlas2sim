#ifndef _GLGRAPHNODE_H_
#define _GLGRAPHNODE_H_

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

/**
* OpenGL graph node
*
* Generates vertices and indices of the sphere used to
* draw the graph's nodes. Builds OpenGL program for
* drawing the nodes. Copies the node data into
* buffers on the GPU. Draws the nodes.
*/
class GLGraphNode: public GLBase
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

	// The Id of node currently selected in the simulation
	unsigned int selectedNode;

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
	// Location of the uniform variable for the selected node in the shaders
	unsigned int uniformSelectedNode;
	// Location of the uniform variable for the camera position in the shaders
	unsigned int uniformCameraPos;

	/**
	* Computes the vertices and indices of the sphere representing a graph's node
	*
	* @param vertices [out] The sphere's vertices
	* @param indices  [out] The sphere's indices
	*/
	void initNode(std::vector<float>* vertices, std::vector<unsigned int>* indices);

public:

	GLGraphNode(const Camera& camera_, const GraphObject& graphObject_);
	~GLGraphNode();

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

	// Gets the number of nodes in the graph
	unsigned int getNumOfNodes() const;
	// Gets the VBO for the sphere's vertices
	unsigned int getVboVertices() const;
	// Gets the VBO for the sphere's indices
	unsigned int getVboIndices() const;
	// Gets the VBO for the nodes' x axis offsets
	unsigned int getVboOffsetX() const;
	// Gets the VBO for the nodes' y axis offsets
	unsigned int getVboOffsetY() const;
	// Gets the VBO for the nodes' z axis offsets
	unsigned int getVboOffsetZ() const;
	// Gets the VBO for the nodes' degrees
	unsigned int getVboScale() const;

};

#endif
