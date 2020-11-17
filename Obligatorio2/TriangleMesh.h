#pragma once
#include "Entity.h"
#include <glm/gtx/vector_angle.hpp>
using namespace std;
class TriangleMesh : public Entity {
public:
	TriangleMesh(glm::vec3 pos, glm::vec3 dir, float esc, GLuint sh);
	~TriangleMesh();
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	void getBounds(glm::vec3& minimum, glm::vec3& maximum);
	GLuint getShaderProgram();
	glm::mat4 getModelMatrix();
	void draw();
private:

	void initTriangleMesh();
};