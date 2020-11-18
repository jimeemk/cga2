#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "FreeImage.h"
#include <glm/gtx/vector_angle.hpp>
using namespace std;

class Plane : public Entity {
public:
	Plane(string tex,int textRepX, int textRepY, glm::vec3 sca, glm::vec3 vertex, glm::vec3 up, glm::vec3 normal, GLuint sh);
	~Plane();
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	GLuint getShaderProgram();
	glm::mat4 getModelMatrix();
	void draw();
private:
	glm::vec3 scale;
	string texture;
	Mesh* mesh;
	int textureRepetitionX;
	int textureRepetitionY;
	void initPlane();
	unsigned int TextureFromFile(const char* path);
};