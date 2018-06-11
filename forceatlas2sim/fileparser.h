#ifndef _FILEPARSER_H_
#define _FILEPARSER_H_

#include <string>

class FileParser
{
private:

	bool endsWith(std::string str, std::string ending);

public:

	FileParser();
	~FileParser();

	void parse(std::string fileName);

};

#endif
