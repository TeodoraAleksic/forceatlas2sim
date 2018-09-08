#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "gltext.h"
#include "shader.h"

GLText::GLText(const Camera& camera_, const GraphObject& graphObject_) : camera(camera_), graphObject(graphObject_)
{
	isInited = false;
	vao = 0;
	vboVertex = 0;
	program = 0;
}

GLText::~GLText()
{
	cleanup();
}

void GLText::initCharacters()
{
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

void GLText::init()
{
	if (isInited)
		return;

	initCharacters();

	// Builds shaders
	std::vector<unsigned int> shaders;
	shaders.push_back(buildShader(GL_VERTEX_SHADER, shader::textVert));
	shaders.push_back(buildShader(GL_FRAGMENT_SHADER, shader::textFrag));

	// Builds program
	program = buildProgram(shaders);

	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		glDeleteShader(*iter);

	// Gets uniform variable locations
	uniformProjection = glGetUniformLocation(program, "projection");
	uniformText = glGetUniformLocation(program, "text");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Allocates buffer for drawing one character
	glGenBuffers(1, &vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	isInited = true;
}

void GLText::draw()
{
}

void GLText::cleanup()
{
	if (!isInited)
		return;

	if (vboVertex)
		glDeleteBuffers(1, &vboVertex);

	if (vao)
		glDeleteVertexArrays(1, &vao);

	if (program)
		glDeleteProgram(program);

	isInited = false;
	vao = 0;
	vboVertex = 0;
	program = 0;
}
