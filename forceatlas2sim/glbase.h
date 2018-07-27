#ifndef _GLBASE_H_
#define _GLBASE_H_

#include <string>
#include <vector>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

class GLBase
{
protected:

	std::string getShaderName(GLenum shaderType);

	unsigned int buildShader(GLenum shaderType, std::string shaderBody);
	unsigned int buildProgram(std::vector<unsigned int> shaders);

public:

	GLBase();
	~GLBase();

	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void cleanup() = 0;

};

#endif
