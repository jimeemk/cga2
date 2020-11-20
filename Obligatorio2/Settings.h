#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "Entity.h"
#include "FreeImage.h"

using namespace std;

class Settings {
private:
	Settings();
	static Settings* instance;
	std::vector<Entity*> entities; //Por ahora es vector porque despues viene the magic
	std::vector<GLuint> shaders;
public:
	~Settings();
	static Settings* getInstance();
	std::vector<Entity*> getEntities();
	std::vector<GLuint> getShaders();
	GLuint addShader(GLuint s);
	void addEntity(Entity* e);
	static GLuint initShaders(const char* vertFile, const char* fragFile);
	static void printShaderError(GLint shader);
	static const char* loadFile(const char* fname);
	static unsigned int TextureFromFile(const char* path);
};
