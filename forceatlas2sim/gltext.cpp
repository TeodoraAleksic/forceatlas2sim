#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "gltext.h"

GLText::GLText(const Camera& camera_, const GraphObject& graphObject_) : camera(camera_), graphObject(graphObject_)
{
	isInited = false;
}

GLText::~GLText()
{
	cleanup();
}

void GLText::init()
{
	if (isInited)
		return;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Could not initialize FreeType" << std::endl;
		throw std::runtime_error("Could not initialize FreeType");
	}

	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	{
		std::cout << "Could not load font" << std::endl;
		throw std::runtime_error("Could not load font");
	}
}

void GLText::draw()
{
}

void GLText::cleanup()
{
	if (!isInited)
		return;

	isInited = false;
}
