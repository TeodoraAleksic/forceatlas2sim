#ifndef _FILEPARSER_H_
#define _FILEPARSER_H_

#include <string>

extern "C" { 
	#include "gml_parser.h" 
}

class FileParser
{
private:

	bool endsWith(std::string str, std::string ending);

	std::string getGMLErrorMessage(GML_error_value error);

	void parseGEFX(std::string fileName);
	void parseGML(std::string fileName);

public:

	FileParser();
	~FileParser();

	void parse(std::string fileName);

};

#endif
