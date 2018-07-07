#ifndef _FILEPARSER_H_
#define _FILEPARSER_H_

#include <string>

#include <libxml/tree.h>

extern "C" 
{
	#include "gml_parser.h" 
}

#include "graphobject.h"

class FileParser
{
private:

	bool endsWith(std::string str, std::string ending);

	void processGEXFNodeAttr(xmlAttr* attr, std::string* id);
	void processGEXFEdgeAttr(xmlAttr* attr, std::string* source, std::string* target);
	void processGEXFNode(xmlNode* node, GraphObject* graphObject);
	void processGEXFEdge(xmlNode* edge, GraphObject* graphObject);
	void processGEXFList(xmlNode* list, GraphObject* graphObject);

	void parseGEXF(std::string fileName, GraphObject* graphObject);

	std::string getGMLErrorMessage(GML_error_value error);

	void processGMLGraphics(GML_pair* graphics, float* x, float* y, float* z);
	void processGMLNode(GML_pair* node, GraphObject* graphObject);
	void processGMLEdge(GML_pair* edge, GraphObject* graphObject);
	void processGMLList(GML_pair* list, GraphObject* graphObject);

	void parseGML(std::string fileName, GraphObject* graphObject);

public:

	FileParser();
	~FileParser();

	GraphObject parse(std::string fileName);

};

#endif
