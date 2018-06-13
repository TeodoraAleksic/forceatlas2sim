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

void FileParser::processGMLGraphics(GML_pair* graphics, float* x, float* y, float* z)
{
	while (graphics != nullptr)
	{
		if (std::strcmp(graphics->key, "x") == 0)
			*x = (float)graphics->value.floating;

		if (std::strcmp(graphics->key, "y") == 0)
			*y = (float)graphics->value.floating;

		if (std::strcmp(graphics->key, "z") == 0)
			*z = (float)graphics->value.floating;

		graphics = graphics->next;
	}
}

void FileParser::processGMLNode(GML_pair* node, GraphObject* graphObject)
{
	std::string id;
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	while (node != nullptr)
	{
		if (std::strcmp(node->key, "id") == 0)
			id = std::to_string(node->value.integer);

		if (std::strcmp(node->key, "graphics") == 0)
			processGMLGraphics(node->value.list, &x, &y, &z);

		node = node->next;
	}

	graphObject->addNode(id, x, y, z);
}

void FileParser::processGMLEdge(GML_pair* edge, GraphObject* graphObject)
{
	std::string source;
	std::string target;

	while (edge != nullptr)
	{
		if (std::strcmp(edge->key, "source") == 0)
			source = std::to_string(edge->value.integer);

		if (std::strcmp(edge->key, "target") == 0)
			target = std::to_string(edge->value.integer);

		// TODO process other fields

		edge = edge->next;
	}

	graphObject->addEdge(source, target);
}

void FileParser::processGMLList(GML_pair* list, GraphObject* graphObject)
{
	GML_pair* tmp = list;

	while (tmp != nullptr && std::strcmp(tmp->key, "graph") != 0)
		tmp = tmp->next;

	if (tmp != nullptr)
	{
		tmp = tmp->value.list;

		while (tmp != nullptr)
		{
			if (std::strcmp(tmp->key, "node") == 0)
				processGMLNode(tmp->value.list, graphObject);

			else if (std::strcmp(tmp->key, "edge") == 0)
				processGMLEdge(tmp->value.list, graphObject);

			tmp = tmp->next;
		}
	}
}

void FileParser::parseGEFX(std::string fileName, GraphObject* graphObject)
{	
	// TODO
}

void FileParser::parseGML(std::string fileName, GraphObject* graphObject)
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

	processGMLList(list, graphObject);

	GML_free_list(list, stat->key_list);
}

GraphObject FileParser::parse(std::string fileName)
{
	GraphObject graphObject;

	if (endsWith(fileName, ".gefx"))
		parseGEFX(fileName, &graphObject);
	else if (endsWith(fileName, ".gml"))
		parseGML(fileName, &graphObject);
	else 
		throw "Unsupported file extension";

	graphObject.postprocessing();

	return graphObject;
}
