#ifndef _FILEPARSER_H_
#define _FILEPARSER_H_

#include <string>

#include <libxml/tree.h>

extern "C" 
{
	#include "gml_parser.h" 
}

#include "graphobject.h"

/**
* Input graph file parser
*
* Parses input GEXF or GML graph file.
* Stores graph data in GraphObject object.
*/
class FileParser
{
private:

	/**
	* Checks if the first string ends with the second string
	*
	* @param str	First string
	* @param ending	Second string
	* @return True if the first ends with the second, else false
	*/
	bool endsWith(std::string str, std::string ending);

	/**
	* Parses node visualization attributes from GEXF file
	*
	* @param attr	[in]	GEFX node visualization attributes
	* @param x		[out]	Coordinate x
	* @param y		[out]	Coordinate y
	* @param z		[out]	Coordinate z
	*/
	void parseGEXFVizAttr(xmlAttr* attr, float* x, float* y, float* z);

	/**
	* Parses node attributes from GEXF file
	*
	* @param attr	[in]	GEFX node attributes
	* @param id		[out]	Node Id
	* @param label	[out]	Node label
	*/
	void parseGEXFNodeAttr(xmlAttr* attr, std::string* id, std::string* label);

	/**
	* Parses edge attributes from GEXF file
	*
	* @param attr	[in]	GEFX edge attributes
	* @param source	[out]	Edge source node Id
	* @param target	[out]	Edge target node Id
	* @param weight [out]	Edge weight
	*/
	void parseGEXFEdgeAttr(xmlAttr* attr, std::string* source, std::string* target, float* weight);

	/**
	* Parses node from GEXF file
	*
	* @param node			[in]	GEFX node
	* @param graphObject	[out]	Object containing graph data
	*/
	void parseGEXFNode(xmlNode* node, GraphObject* graphObject);

	/**
	* Parses edge from GEXF file
	*
	* @param edge			[in]	GEFX edge
	* @param graphObject	[out]	Object containing graph data
	*/
	void parseGEXFEdge(xmlNode* edge, GraphObject* graphObject);

	/**
	* Parses list of node and edge elements from GEXF file
	*
	* @param list			[in]	List of elements
	* @param graphObject	[out]	Object containing graph data
	*/
	void parseGEXFList(xmlNode* list, GraphObject* graphObject);

	/**
	* Parses GEXF file
	*
	* @param fileName		[in]	Input file name
	* @param graphObject	[out]	Object containing graph data
	*/
	void parseGEXF(std::string fileName, GraphObject* graphObject);

	/**
	* Gets GML error message based on error code
	*
	* @param error GML error code
	* @return Corresponding GML error message
	*/
	std::string getGMLErrorMessage(GML_error_value error);

	/**
	* Parses graphics attributes from GML file
	*
	* @param graphics	[in]	GML graphics attributes
	* @param x			[out]	Coordinate x
	* @param y			[out]	Coordinate y
	* @param z			[out]	Coordinate z
	*/
	void parseGMLGraphics(GML_pair* graphics, float* x, float* y, float* z);

	/**
	* Parses node from GML file
	*
	* @param node			[in]	GML node
	* @param graphObject	[out]	Object containing graph data
	*/
	void parseGMLNode(GML_pair* node, GraphObject* graphObject);

	/**
	* Parses edge from GML file
	*
	* @param node			[in]	GML edge
	* @param graphObject	[out]	Object containing graph data
	*/
	void parseGMLEdge(GML_pair* edge, GraphObject* graphObject);

	/**
	* Parses list of node and edge elements from GML file
	*
	* @param list			[in]	List of elements
	* @param graphObject	[out]	Object containing graph data
	*/
	void parseGMLList(GML_pair* list, GraphObject* graphObject);

	/**
	* Parses GML file
	*
	* @param fileName		[in]	Input file name
	* @param graphObject	[out]	Object containing graph data
	*/
	void parseGML(std::string fileName, GraphObject* graphObject);

public:

	FileParser();
	~FileParser();

	/**
	* Parses input GEXF or GML graph file
	*
	* @param fileName Input file name
	* @return Object containing graph data
	*/
	GraphObject parse(std::string fileName);

};

#endif
