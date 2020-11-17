#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Entity {
public:
	Entity();
	Entity(glm::vec3 pos, glm::vec3 dir, float sca, GLuint sp);
	~Entity();
	virtual glm::vec3 getPosition()=0;
	virtual glm::vec3 getDirection()=0;
	virtual void getBounds(glm::vec3 &minimum, glm::vec3 &maximum)=0;
	virtual GLuint getShaderProgram()=0;
	virtual glm::mat4 getModelMatrix() = 0;
	virtual void draw()=0;
protected:
	glm::vec3 min, max;
	glm::vec3 position;
	glm::vec3 direction;
	float scale;
	GLuint shaderProgram;
	glm::mat4 modelMatrix;
	virtual float getMaxScale(float x, float y, float z)=0;
	virtual void rotation()=0;
	glm::vec3 pointsInitialAABB[8];
};