#ifndef _GLTEXT_H_
#define _GLTEXT_H_

#include <map>
#include <string>

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

/**
* Structure representing a character on screen
*/
struct Character 
{
	GLuint textureId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

/**
* OpenGL text
*
* Initializes the FreeType library.
* Reads characters from a font file.
* Draws text on screen.
*/
class GLText : public GLBase
{
private:

	// Text to draw on screen
	std::string text;

	// Camera of the simulation
	const Camera& camera;
	// Object containing graph data
	const GraphObject& graphObject;

	// Indicates if the graph's nodes have already been initialized
	bool isInited;

	// Id of the VAO for all the text's VBOs
	unsigned int vao;

	// Id of the VBO for the text's vertices
	unsigned int vboVertex;

	// Id of the OpenGL program used for drawing the text
	unsigned int program;

	// Location of the uniform variable for the projection matrix in the shaders
	unsigned int uniformProjection;
	// Location of the uniform variable for the text in the shaders
	unsigned int uniformText;

	// Map of characters read from font file
	std::map<GLchar, Character> characters;

	// Reads characters from font file and initializes their parameters
	void initCharacters();

public:

	GLText(const Camera& camera_, const GraphObject& graphObject_);
	~GLText();

	// Initializes the text
	void init();
	// Draws the text
	void draw();
	// Cleans up the text
	void cleanup();

	/**
	* Sets the node currently selected in the simulation
	*
	* @param selectedNode Id of the currently selected node
	*/
	void setSelectedNode(unsigned int selectedNode);

};

#endif
