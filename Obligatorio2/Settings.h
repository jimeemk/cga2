#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "Light.h"
#include "Camera.h"
#include <glad/glad.h>
#include "Shader.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "Entity.h"
#include "FreeImage.h"

using namespace std;
using namespace glm;

class Entity;
class Camera;


class Settings {
private:
	Settings();
	static Settings* instance;
	std::vector<Shader*> shaders;
	Camera* now_camera;
	std::vector<Entity*> entities;
	std::vector<Light*> lights;
	FIBITMAP* terrainTexture;
	static int init_time;
	//bounds 
	vec3 min_bound;
	vec3 max_bound;
public:
	~Settings();
	static Settings* getInstance();
	std::vector<Entity*> getEntities();
	std::vector<Shader*> getShaders();
	Shader* addShader(Shader* s);
	void addEntity(Entity* e);
	void changeNowCamera(Camera* c);
	void setTerrainTexture();
	float getHeightTerrain(float x, float z);
	static GLuint initShaders(const char* vertFile, const char* fragFile);
	static void printShaderError(GLint shader);
	static const char* loadFile(const char* fname);
	void getBounds(vec3&, vec3&);
	Camera* getNowCamera();
	std::vector<Light*> getLights();
	void addLight(Light*);
	int getInitTime();
	static unsigned int TextureFromFile(const char* path);
	vec3 clampToScene(vec3);
	static void SetLightsToShader(Shader* shader);
};
