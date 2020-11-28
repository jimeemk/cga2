#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "FreeImage.h"
#include "SDL.h"
#include "SDL_opengl.h"

using namespace glm;

class Terrain : public Entity
{
public:
	Terrain();
	Terrain(vec3 pos, vec3 u, vec3 dir, Shader* sp, int den, float width, float height,float t, vector<Texture> text);
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	Shader* getShaderProgram();
	glm::mat4 getModelMatrix();
	void draw();
	int getDensity();
	~Terrain();
private:
	int density;
	float width;
	float height;
	float top;
	Mesh* mesh;
	vector<Texture> texture;
	void setupTerrain();
};

