#pragma once
#include "Entity.h"
#include "AnimatedModel.h"
#include <string>
#include <glm/gtx/vector_angle.hpp>
using namespace std;

class AnimatedObject : public Entity {
public:
	AnimatedObject(string p, glm::vec3 ori, float esc, glm::vec3 pos, glm::vec3 up, glm::vec3 dir, GLuint sh);
	~AnimatedObject();
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	GLuint getShaderProgram();
	glm::mat4 getModelMatrix();
	void draw();
private:
	string path;
	AnimatedModel* model;
	glm::vec3 orientation;
	float scale;
	void initObject();
	float getMaxScale(float x, float y, float z);
	void rotation();
};