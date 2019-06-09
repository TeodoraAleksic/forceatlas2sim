#ifndef _GRAPHOBJECT_H_
#define _GRAPHOBJECT_H_

#include <string>
#include <vector>
#include <unordered_map>

class GraphObject
{
private:

	// Number of nodes in graph
	unsigned int numOfNodes;
	// Number of edges in graph
	unsigned int numOfEdges;

	// True if the x axis of at least one node is not 0
	bool initedX;
	// True if the y axis of at least one node is not 0
	bool initedY;
	// True if the z axis of at least one node is not 0
	bool initedZ;

	// Map of node Ids read from the graph file and assigned integer Ids
	std::unordered_map<std::string, unsigned int> nodeIds;

	// Node x axes
	std::vector<float> nodeX;
	// Node y axes
	std::vector<float> nodeY;
	// Node z axes
	std::vector<float> nodeZ;

	// Mean degree of all nodes in the graph
	float meanDegree;
	// Total degree of all nodes in the graph
	float totalDegree;

	// Node degrees
	std::vector<unsigned int> degree;

	// Node labels
	std::vector<std::string> nodeLabel;

	// Number of times a node appears as the source node of an edge
	std::vector<int> sourceNodeCount;
	// Offsets of nodes in the edge source node array
	std::vector<int> sourceNodeOffset;

	// Edge source node Ids
	std::vector<unsigned int> sourceId;
	// Edge target node Ids
	std::vector<unsigned int> targetId;

	// Edge source node x axes
	std::vector<float> sourceX;
	// Edge source node y axes
	std::vector<float> sourceY;
	// Edge source node z axes
	std::vector<float> sourceZ;

	// Edge target node x axes
	std::vector<float> targetX;
	// Edge source node y axes
	std::vector<float> targetY;
	// Edge source node z axes
	std::vector<float> targetZ;

	// Edge weights
	std::vector<float> edgeWeight;

	/**
	* Swaps the values of two integers
	*
	* @param a First integer
	* @param b Second integer
	*/
	void swap(unsigned int* a, unsigned int* b);

	/**
	* Initializes a new node read from the graph file
	*
	* @param node Id of node read from the graph file
	*/
	void initNode(std::string node);

	/**
	* Gets the node's assigned integer Id based on its Id from the graph file
	*
	* @param node Id of node read from the graph file
	* @return Assigned integer Id
	*/
	unsigned int getNodeId(std::string node);

	/**
	* Searches the array of edge source nodes to find
	* the first node whose Id is equal or greater than
	* the Id of the specified source node. Returns -1
	* if it reaches the end of the array.
	*
	* @param source Id of source node
	* @return Index in source node array or -1
	*/
	int findSource(unsigned int source);

	/**
	* Searches the array of edge target nodes to find
	* the first node whose Id is equal or greater than
	* the Id of the specified target node. Returns -1
	* if it reaches the end of the array.
	*
	* @param offset Where from to start the search
	* @param source Id of corresponding source node
	* @param target Id of target node
	* @return Index in target node array or -1
	*/
	int findTarget(int offset, unsigned int source, unsigned int target);

	/**
	* Inserts a new edge and keeps an ascending order of the node arrays
	*
	* @param source Id of source node
	* @param target Id of target node
	* @return Index of the new edge
	*/
	int insertEdgeSorted(unsigned int source, unsigned int target);

	// Post-processes graphics attributes after reading the graph file
	void postprocessGraphics();
	// Post-processes edge attributes after reading the graph file
	void postprocessEdges();

public:

	GraphObject();
	~GraphObject();

	/**
	* Adds new node to the graph
	*
	* @param node	Id of node read from the graph file
	* @param label	Node label
	* @param x		Node x axis
	* @param y		Node y axis
	* @param z		Node z axis
	*/
	void addNode(std::string node, std::string label = "", float x = 0.0, float y = 0.0, float z = 0.0);

	/**
	* Adds new edge to the graph
	*
	* @param source	Id of the edge's source node
	* @param target	Id of the edge's target node
	* @param weight	Edge weight
	*/
	void addEdge(std::string source, std::string target, float weight);

	// Performs post-processing on the graph after file parsing
	void postprocessing();

	/**
	* Calculates the initial distance of the camera from the
	* graph center based on the graph's size and layout
	*
	* @return Distance from the graph center
	*/
	float getInitPosition() const;

	// Gets the number of graph nodes
	unsigned int getNumOfNodes() const;
	// Gets the number of graph edges
	unsigned int getNumOfEdges() const;

	// Gets the total degree of all nodes in the graph
	float getTotalDegree() const;

	// Gets the node x axes
	std::vector<float> getNodeX() const;
	// Gets the node y axes
	std::vector<float> getNodeY() const;
	// Gets the node z axes
	std::vector<float> getNodeZ() const;

	// Gets the node degrees
	std::vector<unsigned int> getNodeDegree() const;

	// Gets the node labels
	std::string getNodeLabel(unsigned int id) const;

	// Gets the offsets of nodes in the edge source node array
	std::vector<int> getSourceNodeOffset() const;

	// Gets the edge source node Ids
	std::vector<unsigned int> getSourceId() const;
	// Gets the edge target node Ids
	std::vector<unsigned int> getTargetId() const;

	// Gets the edge source node x axes
	std::vector<float> getSourceX() const;
	// Gets the edge source node y axes
	std::vector<float> getSourceY() const;
	// Gets the edge source node z axes
	std::vector<float> getSourceZ() const;

	// Gets the edge target node x axes
	std::vector<float> getTargetX() const;
	// Gets the edge target node y axes 
	std::vector<float> getTargetY() const;
	// Gets the edge target node z axes
	std::vector<float> getTargetZ() const;

	// Gets the edge weights
	std::vector<float> getEdgeWeight() const;

};

#endif
