#pragma once

#include "Entity.h"
#include "Mesh.h"
#include "FreeImage.h"
#include "SDL.h"
#include "SDL_opengl.h"

#define NUMBERWAVES 4

using namespace glm;

class Water : public Entity
{
public:
	Water();
	Water(vec3 pos, vec3 u, vec3 dir, Shader* sp, int den, float width, float height, std::vector<Texture> texture);
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

	bool intersectionBoxBounds(vec3, vec3);
private:
	int density;
	float width;
	float height;
	Mesh* mesh;
	std::vector<Texture> texture;
	void setupWater();
	int last_time;
	float c;
	unsigned int terrainColorTexture;
	vec4 waveParameters[NUMBERWAVES];
	vec2 waveDirections[NUMBERWAVES];
	void setWavesValues();
	unsigned int id;
};

