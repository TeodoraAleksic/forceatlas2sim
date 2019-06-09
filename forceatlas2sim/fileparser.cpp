#include <iostream>
#include <stdio.h>

#include <libxml/parser.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "fileparser.h"
#include "message.h"
#include "utility.h"

FileParser::FileParser()
{
}

FileParser::~FileParser()
{
}

bool FileParser::endsWith(std::string str, std::string ending)
{
	if (str.length() >= ending.length()) 
		// Returns true if first string ends with the second string
		return str.compare(str.length() - ending.length(), ending.length(), ending) == 0;
	else 
		return false;
}

void FileParser::parseGEXFVizAttr(xmlAttr* attr, float* x, float* y, float* z)
{
	// Processes visualization attribute section of GEXF file
	while (attr != nullptr)
	{
		if (attr->type == XML_ATTRIBUTE_NODE)
		{
			if (std::strcmp((const char*)attr->name, "x") == 0)
				*x = std::stof((const char*)attr->children->content);

			if (std::strcmp((const char*)attr->name, "y") == 0)
				*y = std::stof((const char*)attr->children->content);

			if (std::strcmp((const char*)attr->name, "z") == 0)
				*z = std::stof((const char*)attr->children->content);
		}

		attr = attr->next;
	}
}

void FileParser::parseGEXFNodeAttr(xmlAttr* attr, std::string* id, std::string* label)
{
	// Processes node attribute section of GEXF file
	while (attr != nullptr)
	{
		if (attr->type == XML_ATTRIBUTE_NODE) 
		{
			if (std::strcmp((const char*)attr->name, "id") == 0)
				*id = (const char*)attr->children->content;

			if (std::strcmp((const char*)attr->name, "label") == 0)
				*label = (const char*)attr->children->content;
		}

		attr = attr->next;
	}
}

void FileParser::parseGEXFEdgeAttr(xmlAttr* attr, std::string* source, std::string* target, float* weight)
{
	// Processes edge attribute section of GEXF file
	while (attr != nullptr)
	{
		if (attr->type == XML_ATTRIBUTE_NODE)
		{
			if (std::strcmp((const char*)attr->name, "source") == 0)
				*source = (const char*)attr->children->content;

			if (std::strcmp((const char*)attr->name, "target") == 0)
				*target = (const char*)attr->children->content;

			if (std::strcmp((const char*)attr->name, "cardinal") == 0)
				*weight = std::stof((const char*)attr->children->content);
		}

		attr = attr->next;
	}
}

void FileParser::parseGEXFNode(xmlNode* node, GraphObject* graphObject)
{
	// Processes node section of GEXF file
	while (node != nullptr)
	{
		if (node->type == XML_ELEMENT_NODE && std::strcmp((const char*)node->name, "node") == 0)
		{
			std::string id;
			std::string label;
			float x = 0.0;
			float y = 0.0;
			float z = 0.0;

			parseGEXFNodeAttr(node->properties, &id, &label);
			
			xmlNode* temp = node->children;

			while (temp != nullptr)
			{
				if (temp->type == XML_ELEMENT_NODE)
				{
					// Process node hierarchy
					if (std::strcmp((const char*)temp->name, "nodes") == 0)
						parseGEXFNode(temp->children, graphObject);

					if (std::strcmp((const char*)temp->name, "position") == 0)
						parseGEXFVizAttr(temp->properties, &x, &y, &z);
				}

				temp = temp->next;
			}

			graphObject->addNode(id, label, x, y, z);
		}

		node = node->next;
	}
}

void FileParser::parseGEXFEdge(xmlNode* edge, GraphObject* graphObject)
{
	// Processes edge section of GEXF file
	while (edge != nullptr)
	{
		if (edge->type == XML_ELEMENT_NODE && std::strcmp((const char*)edge->name, "edge") == 0)
		{
			std::string source;
			std::string target;
			float weight = 1.0;

			parseGEXFEdgeAttr(edge->properties, &source, &target, &weight);

			graphObject->addEdge(source, target, weight);
		}

		edge = edge->next;
	}
}

void FileParser::parseGEXFList(xmlNode* list, GraphObject* graphObject)
{
	list = list->children;

	// Finds the graph section of a GEXF file
	while (list != nullptr && 
		   (list->type != XML_ELEMENT_NODE || 
		    (list->type == XML_ELEMENT_NODE && std::strcmp((const char*)list->name, "graph") != 0)))
		list = list->next;

	if (list != nullptr)
	{
		list = list->children;

		// Processes graph section of GEXF file
		while (list != nullptr)
		{
			if (list->type == XML_ELEMENT_NODE)
			{
				if (std::strcmp((const char*)list->name, "nodes") == 0)
					parseGEXFNode(list->children, graphObject);

				else if (std::strcmp((const char*)list->name, "edges") == 0)
					parseGEXFEdge(list->children, graphObject);
			}

			list = list->next;
		}
	}
}

void FileParser::parseGEXF(std::string fileName, GraphObject* graphObject)
{
#ifdef LIBXML_TREE_ENABLED
	xmlDoc *doc = nullptr;
	xmlNode *rootElement = nullptr;

	LIBXML_TEST_VERSION

	doc = xmlReadFile(fileName.c_str(), nullptr, 0);

	if (doc == nullptr)
		logAndThrow(msg::ERR_READING_GEXF_FILE);

	rootElement = xmlDocGetRootElement(doc);

	parseGEXFList(rootElement, graphObject);

	xmlFreeDoc(doc);
	xmlCleanupParser();
#else
	logAndThrow(msg::ERR_GEXF_TREE_SUPPORT);
#endif
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

void FileParser::parseGMLGraphics(GML_pair* graphics, float* x, float* y, float* z)
{
	// Processes graphics sections of GML file
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

void FileParser::parseGMLNode(GML_pair* node, GraphObject* graphObject)
{
	std::string id;
	std::string label;
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	// Processes node section of GML file
	while (node != nullptr)
	{
		if (std::strcmp(node->key, "id") == 0)
			id = std::to_string(node->value.integer);

		if (std::strcmp(node->key, "label") == 0)
			label = std::string(node->value.string);

		if (std::strcmp(node->key, "graphics") == 0)
			parseGMLGraphics(node->value.list, &x, &y, &z);

		node = node->next;
	}

	graphObject->addNode(id, label, x, y, z);
}

void FileParser::parseGMLEdge(GML_pair* edge, GraphObject* graphObject)
{
	std::string source;
	std::string target;
	float weight = 1;

	// Processes edge section of GML file
	while (edge != nullptr)
	{
		if (std::strcmp(edge->key, "source") == 0)
			source = std::to_string(edge->value.integer);

		if (std::strcmp(edge->key, "target") == 0)
			target = std::to_string(edge->value.integer);

		if (std::strcmp(edge->key, "value") == 0)
			weight = edge->value.floating >= 0.0f ? 
			(float)edge->value.floating : (float)edge->value.integer;

		edge = edge->next;
	}

	graphObject->addEdge(source, target, weight);
}

void FileParser::parseGMLList(GML_pair* list, GraphObject* graphObject)
{
	// Finds the graph section of a GML file
	while (list != nullptr && std::strcmp(list->key, "graph") != 0)
		list = list->next;

	if (list != nullptr)
	{
		list = list->value.list;

		// Processes graph section of GML file
		while (list != nullptr)
		{
			if (std::strcmp(list->key, "node") == 0)
				parseGMLNode(list->value.list, graphObject);

			else if (std::strcmp(list->key, "edge") == 0)
				parseGMLEdge(list->value.list, graphObject);

			list = list->next;
		}
	}
}

void FileParser::parseGML(std::string fileName, GraphObject* graphObject)
{
	// Opens input file for reading
	FILE* file = fopen(fileName.c_str(), "r");
	if (file == nullptr)
		logAndThrow(fmt::format(msg::ERR_READING_GRAPH_FILE, std::strerror(errno)));

	// Initializes GML structures
	GML_init();
	GML_stat stat;
	stat.key_list = NULL;
	GML_pair* list = GML_parser(file, &stat, 0);

	// Closes input file
	if (file != nullptr) fclose(file);

	// Checks for errors during GML initialization
	if (stat.err.err_num != GML_OK)
		logAndThrow(fmt::format(msg::ERR_READING_GML_FILE, 
			stat.err.line, stat.err.column, getGMLErrorMessage(stat.err.err_num)));
	
	parseGMLList(list, graphObject);

	GML_free_list(list, stat.key_list);
}

GraphObject FileParser::parse(std::string fileName)
{
	GraphObject graphObject;

	spdlog::info(fmt::format(msg::INFO_READING_GRAPH_FILE, fileName));

	// Parses input file based on extension
	if (endsWith(fileName, ".gexf"))
		parseGEXF(fileName, &graphObject);
	else if (endsWith(fileName, ".gml"))
		parseGML(fileName, &graphObject);
	else
		logAndThrow(fmt::format(msg::ERR_GRAPH_FILE_EXT, fileName));

	spdlog::info(fmt::format(msg::INFO_NUM_OF_NODES, graphObject.getNumOfNodes()));
	spdlog::info(fmt::format(msg::INFO_NUM_OF_EDGES, graphObject.getNumOfEdges()));

	graphObject.postprocessing();

	return graphObject;
}
