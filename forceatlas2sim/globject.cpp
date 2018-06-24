#include "globject.h"

#include <iostream>

GLObject::GLObject()
{
}

GLObject::~GLObject()
{
}

std::string GLObject::getShaderName(GLenum shaderType)
{
	switch (shaderType)
	{
	case GL_COMPUTE_SHADER:
		return "GL_COMPUTE_SHADER";
	case GL_GEOMETRY_SHADER:
		return "GL_GEOMETRY_SHADER";
	case GL_FRAGMENT_SHADER:
		return "GL_FRAGMENT_SHADER";
	case GL_TESS_CONTROL_SHADER:
		return "GL_TESS_CONTROL_SHADER";
	case GL_TESS_EVALUATION_SHADER:
		return "GL_TESS_EVALUATION_SHADER";
	case GL_VERTEX_SHADER:
		return "GL_VERTEX_SHADER";
	default:
		return "UNKNOWN";
	}
}

unsigned int GLObject::buildShader(GLenum shaderType, std::string shaderBody)
{
	unsigned int shader;
	shader = glCreateShader(shaderType);

	// Builds shader from source
	const char* shaderSource = shaderBody.c_str();
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	// Gets build status
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::" << getShaderName(shaderType) << "::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	return shader;
}

unsigned int GLObject::buildProgram(std::vector<unsigned int> shaders)
{
	unsigned int program;
	program = glCreateProgram();

	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		glAttachShader(program, *iter);

	glLinkProgram(program);

	// Gets build status
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) 
	{
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
	}

	return program;
}
