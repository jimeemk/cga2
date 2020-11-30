#pragma once

#include "Entity.h"
#include "Mesh.h"
#include "FreeImage.h"
#include "SDL.h"
#include "SDL_opengl.h"

using namespace glm;

class Water : public Entity
{
public:
	Water();
	Water(vec3 pos, vec3 u, vec3 dir, Shader* sp, int den, float width, float height, std::string text);
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	Shader* getShaderProgram();
	glm::mat4 getModelMatrix();
	void draw();
	int getDensity();
	~Water();
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
	int density;
	float width;
	float height;
	Mesh* mesh;
	std::string texture;
	void setupWater();
	int last_time;
	float c;
};

