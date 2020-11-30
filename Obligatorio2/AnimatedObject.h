#pragma once
#include "Entity.h"
#include "AnimatedModel.h"
#include <string>
#include <glm/gtx/vector_angle.hpp>
using namespace std;

class AnimatedObject : public Entity {
public:
	AnimatedObject(string p, glm::vec3 ori, float esc, glm::vec3 pos, glm::vec3 up, glm::vec3 dir, Shader* sh);
	~AnimatedObject();
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	Shader* getShaderProgram();
	glm::mat4 getModelMatrix();
	void draw();
	void aumentarX(); //r
	void disminuirX(); //f
	void aumentarY(); //t
	void disminuirY();//g
	void aumentarZ();//y
	void disminuirZ(); //h
	void rotarMasX(); //u
	void rotarMenosX();//j
	void rotarMasY(); //i
	void rotarMenosY(); //k
	void rotarMasZ();//o
	void rotarMenosZ();//l
	void escMasX();//1
	void escMenosX();//2
	void escMasY();//3
	void escMenosY();//4
	void escMasZ();//5
	void escMenosZ();//6
	void guardarEntity();//7
private:
	string path;
	AnimatedModel* model;
	glm::vec3 orientation;
	float scale;
	void initObject();
	float getMaxScale(float x, float y, float z);
	void rotation();
};