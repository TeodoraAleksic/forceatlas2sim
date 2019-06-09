#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glselect.h"
#include "shader.h"

const int GLSelect::lats = 40;
const int GLSelect::longs = 40;
const int GLSelect::numOfIndices = (lats + 1) * (longs + 1) * 2 + (lats + 1);

GLSelect::GLSelect(const Camera& camera_, const GraphObject& graphObject_):
	camera(camera_), graphObject(graphObject_),
	isInited(false),
	vao(0),
	vboVertex(0), vboIndex(0),
	vboOffsetX(0), vboOffsetY(0), vboOffsetZ(0),
	vboScale(0),
	program(0),
	uniformProjection(0),
	uniformView(0),
	uniformModel(0),
	uniformNormalMatrix(0),
	uniformCameraPos(0)
{
}

GLSelect::~GLSelect()
{
	cleanup();
}

void GLSelect::init()
{
	if (isInited)
		return;

	// Builds shaders
	std::vector<unsigned int> shaders;
	shaders.push_back(buildShader(GL_VERTEX_SHADER, shader::selectVert));
	shaders.push_back(buildShader(GL_FRAGMENT_SHADER, shader::selectFrag));

	// Builds program
	program = buildProgram(shaders);

	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		glDeleteShader(*iter);

	// Gets uniform variable locations
	uniformProjection = glGetUniformLocation(program, "projection");
	uniformView = glGetUniformLocation(program, "view");
	uniformModel = glGetUniformLocation(program, "model");
	uniformNormalMatrix = glGetUniformLocation(program, "normalMatrix");
	uniformCameraPos = glGetUniformLocation(program, "cameraPos");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Binds vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Binds indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);

	// Binds offsets
	glBindBuffer(GL_ARRAY_BUFFER, vboOffsetX);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vboOffsetY);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vboOffsetZ);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(3, 1);

	// Binds scale
	glBindBuffer(GL_ARRAY_BUFFER, vboScale);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(4, 1);

	isInited = true;
}

void GLSelect::draw()
{
	if (!isInited)
		return;

	glUseProgram(program);

	glm::mat4 projection = camera.getProjectionMatrix();
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 model(1.0f);
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(model));
	glm::vec3 cameraPos = camera.getCameraPos();

	// Sets uniforms for camera position
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(uniformNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform3f(uniformCameraPos, cameraPos.x, cameraPos.y, cameraPos.z);

	// Binds buffers
	glBindVertexArray(vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);

	// Draws nodes
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 2 * numOfIndices, GL_UNSIGNED_INT, NULL, graphObject.getNumOfNodes());

	glFinish();
}

void GLSelect::cleanup()
{
	if (!isInited)
		return;

	if (vao)
		glDeleteVertexArrays(1, &vao);

	if (program)
		glDeleteProgram(program);

	isInited = false;
	vao = 0;
	vboVertex = 0;
	vboIndex = 0;
	vboOffsetX = 0;
	vboOffsetY = 0;
	vboOffsetZ = 0;
	vboScale = 0;
	program = 0;
}

void GLSelect::setVboVertex(unsigned int vboVertex_)
{
	vboVertex = vboVertex_;
}

void GLSelect::setVboIndex(unsigned int vboIndex_)
{
	vboIndex = vboIndex_;
}

void GLSelect::setVboOffsetX(unsigned int vboOffsetX_)
{
	vboOffsetX = vboOffsetX_;
}

void GLSelect::setVboOffsetY(unsigned int vboOffsetY_)
{
	vboOffsetY = vboOffsetY_;
}

void GLSelect::setVboOffsetZ(unsigned int vboOffsetZ_)
{
	vboOffsetZ = vboOffsetZ_;
}

void GLSelect::setVboScale(unsigned int vboScale_)
{
	vboScale = vboScale_;
}
