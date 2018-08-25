#include <iostream>
#include <stdio.h>

#include <libxml/parser.h>

#include "fileparser.h"

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

void FileParser::processGEXFVizAttr(xmlAttr* attr, float* x, float* y, float* z)
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

void FileParser::processGEXFNodeAttr(xmlAttr* attr, std::string* id)
{
	// Processes node attribute section of GEXF file
	while (attr != nullptr)
	{
		if (attr->type == XML_ATTRIBUTE_NODE) 
		{
			if (std::strcmp((const char*)attr->name, "id") == 0)
				*id = (const char*)attr->children->content;
		}

		attr = attr->next;
	}
}

void FileParser::processGEXFEdgeAttr(xmlAttr* attr, std::string* source, std::string* target, float* weight)
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

			if (std::strcmp((const char*)attr->name, "weight") == 0)
				*weight = std::stof((const char*)attr->children->content);
		}

		attr = attr->next;
	}
}

void FileParser::processGEXFNode(xmlNode* node, GraphObject* graphObject)
{
	// Processes node section of GEXF file
	while (node != nullptr)
	{
		if (node->type == XML_ELEMENT_NODE && std::strcmp((const char*)node->name, "node") == 0)
		{
			std::string id;
			float x = 0.0;
			float y = 0.0;
			float z = 0.0;

			processGEXFNodeAttr(node->properties, &id);
			
			xmlNode* temp = node->children;

			while (temp != nullptr)
			{
				if (temp->type == XML_ELEMENT_NODE)
				{
					// Process node hierarchy
					if (std::strcmp((const char*)temp->name, "nodes") == 0)
						processGEXFNode(temp->children, graphObject);

					if (std::strcmp((const char*)temp->name, "position") == 0)
						processGEXFVizAttr(temp->properties, &x, &y, &z);
				}

				temp = temp->next;
			}

			graphObject->addNode(id, x, y, z);
		}

		node = node->next;
	}
}

void FileParser::processGEXFEdge(xmlNode* edge, GraphObject* graphObject)
{
	// Processes edge section of GEXF file
	while (edge != nullptr)
	{
		if (edge->type == XML_ELEMENT_NODE && std::strcmp((const char*)edge->name, "edge") == 0)
		{
			std::string source;
			std::string target;
			float weight = 1.0;

			processGEXFEdgeAttr(edge->properties, &source, &target, &weight);

			graphObject->addEdge(source, target, weight);
		}

		edge = edge->next;
	}
}

void FileParser::processGEXFList(xmlNode* list, GraphObject* graphObject)
{
	xmlNode* tmp = list->children;

	// Finds the graph section of a GEXF file
	while (tmp != nullptr && 
		   (tmp->type != XML_ELEMENT_NODE || 
		    (tmp->type == XML_ELEMENT_NODE && std::strcmp((const char*)tmp->name, "graph") != 0)))
		tmp = tmp->next;

	if (tmp != nullptr)
	{
		tmp = tmp->children;

		// Processes graph section of GEXF file
		while (tmp != nullptr)
		{
			if (tmp->type == XML_ELEMENT_NODE)
			{
				if (std::strcmp((const char*)tmp->name, "nodes") == 0)
					processGEXFNode(tmp->children, graphObject);

				else if (std::strcmp((const char*)tmp->name, "edges") == 0)
					processGEXFEdge(tmp->children, graphObject);
			}

			tmp = tmp->next;
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
		throw "Could not parse GEXF file";

	rootElement = xmlDocGetRootElement(doc);

	processGEXFList(rootElement, graphObject);

	xmlFreeDoc(doc);
	xmlCleanupParser();
#else
	throw "GEXF tree support not compiled";
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

void FileParser::processGMLGraphics(GML_pair* graphics, float* x, float* y, float* z)
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

void FileParser::processGMLNode(GML_pair* node, GraphObject* graphObject)
{
	std::string id;
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	// Processes node section of GML file
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

void FileParser::processGMLList(GML_pair* list, GraphObject* graphObject)
{
	GML_pair* tmp = list;

	// Finds the graph section of a GML file
	while (tmp != nullptr && std::strcmp(tmp->key, "graph") != 0)
		tmp = tmp->next;

	if (tmp != nullptr)
	{
		tmp = tmp->value.list;

		// Processes graph section of GML file
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

	// Parses input file based on extension
	if (endsWith(fileName, ".gexf"))
		parseGEXF(fileName, &graphObject);
	else if (endsWith(fileName, ".gml"))
		parseGML(fileName, &graphObject);
	else 
		throw "Unsupported file extension";

	graphObject.postprocessing();

	return graphObject;
}
