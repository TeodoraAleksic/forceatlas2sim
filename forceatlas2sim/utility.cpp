#include "utility.h"

#include <fstream>
#include <iostream>
#include <streambuf>

std::string readFile(std::string fileName)
{
	std::ifstream file(fileName);

	if (!file) throw "Failed to open file " + fileName;

	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return fileContent;
}
