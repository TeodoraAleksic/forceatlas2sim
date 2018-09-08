#ifndef _GLTEXT_H_
#define _GLTEXT_H_

#include <map>

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

struct Character {
	GLuint textureId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

class GLText : public GLBase
{
private:

	const Camera& camera;
	const GraphObject& graphObject;

	bool isInited;

	std::map<GLchar, Character> characters;

public:

	GLText(const Camera& camera_, const GraphObject& graphObject_);
	~GLText();

	void init();
	void draw();
	void cleanup();

};

#endif
