#include "fileparser.h"

#include <iostream>

FileParser::FileParser()
{
}

FileParser::~FileParser()
{
}

bool FileParser::endsWith(std::string str, std::string ending)
{
	if (str.length() >= ending.length()) 
		return str.compare(str.length() - ending.length(), ending.length(), ending) == 0;
	else 
		return false;
}

void FileParser::parse(std::string fileName)
{
	if (endsWith(fileName, ".gefx"))
	{
		// TODO
		std::cout << "GEFX\n";
	}
	else if (endsWith(fileName, ".gml"))
	{
		// TODO
		std::cout << "GML\n";
	}
	else 
	{
		// TODO
		std::cout << "Unsupported file extension\n";
	}
}
