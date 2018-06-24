#ifndef _GLOBJECT_H_
#define _GLOBJECT_H_

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <string>
#include <vector>

class GLObject
{
protected:

	std::string getShaderName(GLenum shaderType);

	unsigned int buildShader(GLenum shaderType, std::string shaderBody);
	unsigned int buildProgram(std::vector<unsigned int> shaders);

public:

	GLObject();
	~GLObject();

	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void cleanup() = 0;

};

#endif
