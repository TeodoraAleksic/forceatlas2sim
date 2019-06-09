#ifndef _GLBASE_H_
#define _GLBASE_H_

#include <string>
#include <vector>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

/**
* Base OpenGL class
*
* Contains functionalities used by
* all other OpenGL classes.
*/
class GLBase
{
protected:

	/**
	* Gets the name of a shader's type
	*
	* @param shaderType The shader's type
	* @return The name of a shader's type
	*/
	std::string getShaderName(GLenum shaderType);

	/**
	* Builds a shader
	*
	* @param shaderType The shader's type
	* @param shaderBody The shader's body
	* @return The Id of the built shader
	*/
	unsigned int buildShader(GLenum shaderType, std::string shaderBody);
	
	/**
	* Builds an OpenGL program
	*
	* @param shaders List of built shaders to attach to program
	* @return The Id of the built program
	*/
	unsigned int buildProgram(std::vector<unsigned int> shaders);

public:

	GLBase();
	~GLBase();

	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void cleanup() = 0;

};

#endif
