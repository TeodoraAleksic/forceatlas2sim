#include "fileparser.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

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

std::string FileParser::getGMLErrorMessage(GML_error_value error)
{
	switch (error) 
	{
	case GML_UNEXPECTED:
		return "UNEXPECTED CHARACTER";
	case GML_SYNTAX:
		return "SYNTAX ERROR";
	case GML_PREMATURE_EOF:
		return "PREMATURE EOF IN STRING";
	case GML_TOO_MANY_DIGITS:
		return "NUMBER WITH TOO MANY DIGITS";
	case GML_OPEN_BRACKET:
		return "OPEN BRACKETS LEFT AT EOF";
	case GML_TOO_MANY_BRACKETS:
		return "TOO MANY CLOSING BRACKETS";
	default:
		return "UNKNOWN";
	}
}

void FileParser::parseGEFX(std::string fileName)
{
	// TODO
}

void FileParser::parseGML(std::string fileName)
{
	struct GML_pair* list;
	struct GML_stat* stat = (struct GML_stat*)malloc(sizeof(struct GML_stat));
	stat->key_list = NULL;

	FILE* file = fopen(fileName.c_str(), "r");

	GML_init();
	list = GML_parser(file, stat, 0);

	if (file != nullptr) fclose(file);

	if (stat->err.err_num != GML_OK)
	{
		std::cout << getGMLErrorMessage(stat->err.err_num) << "\n";
		std::cout << "Error reading file line " << stat->err.line << " column " << stat->err.column << "\n";
	}

	// TODO

	GML_free_list(list, stat->key_list);
}

void FileParser::parse(std::string fileName)
{
	if (endsWith(fileName, ".gefx"))
	{
		parseGEFX(fileName);
	}
	else if (endsWith(fileName, ".gml"))
	{
		parseGML(fileName);
	}
	else 
	{
		// TODO
		std::cout << "Unsupported file extension\n";
	}
}
