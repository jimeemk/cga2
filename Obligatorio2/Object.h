#pragma once
#include "Entity.h"
#include "Model.h"
#include <string>
#include <glm/gtx/vector_angle.hpp>
using namespace std;

class Object: public Entity {
public:
	Object(string p, glm::vec3 ori, glm::vec3 up, glm::vec3 pos, glm::vec3 dir, float esc, GLuint sh, Shader s);
	~Object();
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	void getBounds(glm::vec3& minimum, glm::vec3& maximum);
	GLuint getShaderProgram();
	glm::mat4 getModelMatrix();
	void draw();
private:
	string path;
	Model* model;
	glm::vec3 orientation;
	glm::vec3 up;
	Shader shader;
	void initObject();
	float getMaxScale(float x, float y, float z);
	void rotation();
};