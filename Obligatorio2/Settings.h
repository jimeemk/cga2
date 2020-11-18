#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "Entity.h"

using namespace std;

class Settings {
private:
	Settings();
	static Settings* instance;
	std::vector<Entity*> entities; //Por ahora es vector porque despues viene the magic
	std::vector<Shader> shaders;
public:
	~Settings();
	static Settings* getInstance();
	std::vector<Entity*> getEntities();
	std::vector<Shader> getShaders();
	Shader addShader(Shader s);
	void addEntity(Entity* e);
	static GLuint initShaders(const char* vertFile, const char* fragFile);
	static void printShaderError(GLint shader);
	static const char* loadFile(const char* fname);
};
