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

	FT_Set_Pixel_Sizes(face, 0, 18);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Could not load character " << c << std::endl;
			throw std::runtime_error("Could not load character");
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};

		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
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
