#ifndef _GLTEXT_H_
#define _GLTEXT_H_

#include <map>
#include <string>

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

struct Character 
{
	GLuint textureId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

class GLText : public GLBase
{
private:

	std::string text;

	const Camera& camera;
	const GraphObject& graphObject;

	bool isInited;

	unsigned int vao;
	unsigned int vboVertex;

	unsigned int program;
	unsigned int uniformProjection, uniformText;

	std::map<GLchar, Character> characters;

	void initCharacters();

public:

	GLText(const Camera& camera_, const GraphObject& graphObject_);
	~GLText();

	void init();
	void draw();
	void cleanup();

	void setSelectedNode(unsigned int selectedNode);

};

#endif
