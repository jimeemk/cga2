#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(glm::vec3 pos, glm::vec3 dir, float esc, GLuint sh):Entity(pos,dir,esc,sh)
{
}

TriangleMesh::~TriangleMesh()
{
}

glm::vec3 TriangleMesh::getPosition()
{
	return position;
}

glm::vec3 TriangleMesh::getDirection()
{
	return direction;
}

void TriangleMesh::getBounds(glm::vec3& minimum, glm::vec3& maximum)
{
}

GLuint TriangleMesh::getShaderProgram()
{
	return shaderProgram;
}

glm::mat4 TriangleMesh::getModelMatrix()
{
	return modelMatrix;
}

void TriangleMesh::draw()
{
}

void TriangleMesh::initTriangleMesh()
{
}
