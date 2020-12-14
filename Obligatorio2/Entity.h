#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "FreeImage.h"
#include "btBulletDynamicsCommon.h"

#include "Shader.h"
#include "Settings.h"

using namespace std;
class Entity {
public:
	Entity();
	Entity(glm::vec3 pos, glm::vec3 u, glm::vec3 dir, Shader* sp);
	~Entity();
	virtual glm::vec3 getPosition()=0;
	virtual glm::vec3 getDirection()=0;
	virtual void getBounds(glm::vec3 &minimum, glm::vec3 &maximum);
	virtual void calcBounds();
	virtual void getSphericalBounds(glm::vec3&, float&);
	virtual Shader* getShaderProgram()=0;
	virtual glm::mat4 getModelMatrix()= 0;
	virtual void draw()=0;
	void drawBounds();
	virtual void aumentarX()=0; //r
	virtual void disminuirX() = 0; //f
	virtual void aumentarY() = 0; //t
	virtual void disminuirY() = 0;//g
	virtual void aumentarZ() = 0;//y
	virtual void disminuirZ() = 0; //h
	virtual void rotarMasX() = 0; //u
	virtual void rotarMenosX() = 0;//j
	virtual void rotarMasY() = 0; //i
	virtual void rotarMenosY() = 0; //k
	virtual void rotarMasZ() = 0;//o
	virtual void rotarMenosZ() = 0;//l
	virtual void escMasX() = 0;//1
	virtual void escMenosX() = 0;//2
	virtual void escMasY() = 0;//3
	virtual void escMenosY() = 0;//4
	virtual void escMasZ() = 0;//5
	virtual void escMenosZ() = 0;//6
	virtual void guardarEntity() = 0;//7

	virtual bool intersectionBoxBounds(vec3, vec3);

protected:
	glm::vec3 min, max;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	Shader* shaderProgram;
	glm::mat4 modelMatrix;
	glm::vec3 pointsInitialAABB[8];
	unsigned int TextureFromFile(const char* path);
};