#ifndef _GLTEXT_H_
#define _GLTEXT_H_

#include "camera.h"
#include "glbase.h"
#include "graphobject.h"

class GLText : public GLBase
{
private:

	const Camera& camera;
	const GraphObject& graphObject;

	bool isInited;

public:

	GLText(const Camera& camera_, const GraphObject& graphObject_);
	~GLText();

	void init();
	void draw();
	void cleanup();

};

#endif
