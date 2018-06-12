#ifndef _FILEPARSER_H_
#define _FILEPARSER_H_

#include <string>

#include "graphobject.h"

extern "C" { 
	#include "gml_parser.h" 
}

class FileParser
{
private:

	bool endsWith(std::string str, std::string ending);

	std::string getGMLErrorMessage(GML_error_value error);
	std::string getGMLElementValue(GML_pair element);

	void processGMLNode(GML_pair* node, GraphObject* graphObject);
	void processGMLEdge(GML_pair* edge, GraphObject* graphObject);
	void processGMLList(GML_pair* list, GraphObject* graphObject);

	void parseGEFX(std::string fileName, GraphObject* graphObject);
	void parseGML(std::string fileName, GraphObject* graphObject);

public:

	FileParser();
	~FileParser();

	GraphObject parse(std::string fileName);

};

#endif
