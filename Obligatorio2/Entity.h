#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

using namespace std;
class Entity {
public:
	Entity();
	Entity(glm::vec3 pos, glm::vec3 u, glm::vec3 dir, Shader sp);
	~Entity();
	virtual glm::vec3 getPosition()=0;
	virtual glm::vec3 getDirection()=0;
	virtual void getBounds(glm::vec3 &minimum, glm::vec3 &maximum);
	virtual void calcBounds();
	virtual Shader getShaderProgram()=0;
	virtual glm::mat4 getModelMatrix()= 0;
	virtual void draw()=0;
protected:
	glm::vec3 min, max;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	Shader shaderProgram;
	glm::mat4 modelMatrix;
	glm::vec3 pointsInitialAABB[8];
};