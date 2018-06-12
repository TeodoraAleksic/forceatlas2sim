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

std::string FileParser::getGMLElementValue(GML_pair element)
{
	switch (element.kind)
	{
	case GML_STRING:
		return element.value.string;
	case GML_INT:
		return std::to_string(element.value.integer);
	case GML_DOUBLE:
		return std::to_string(element.value.floating);
	default:
		throw "GML element value is not a primitive type";
	}
}

void FileParser::processGMLNode(GML_pair* node, GraphObject* graphObject)
{
	while (node != nullptr)
	{
		if (std::strcmp(node->key, "id") == 0)
			graphObject->addNode(getGMLElementValue(*node));

		// TODO process other fields

		node = node->next;
	}
}

void FileParser::processGMLEdge(GML_pair* edge, GraphObject* graphObject)
{
	std::string source;
	std::string target;

	while (edge != nullptr)
	{
		if (std::strcmp(edge->key, "source") == 0)
			source = getGMLElementValue(*edge);

		if (std::strcmp(edge->key, "target") == 0)
			target = getGMLElementValue(*edge);

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

	return graphObject;
}
