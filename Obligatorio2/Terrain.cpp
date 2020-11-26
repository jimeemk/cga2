#include "Terrain.h"


Terrain::Terrain()
{
}

Terrain::Terrain(vec3 pos, vec3 u, vec3 dir, Shader* sp, int den, float width, float height, std::string text) : Entity(pos, normalize(u), normalize(dir), sp)
{
	density = den;
	this->width = width;
	this->height = height;
	texture = text;
	modelMatrix = mat4(1.f);
	setupTerrain();
}

void Terrain::setupTerrain()
{
	vec3 v = normalize(cross(up, direction));
	vec3 u = normalize(cross(direction, v));
	//vectores
	std::vector<Vertex> vertices;
	std::vector<unsigned int>indices;
	std::vector<Texture>textures;
	Texture t = { TextureFromFile(texture.c_str()), "texture_height", texture };
	textures.push_back(t);
	Texture t2 = { TextureFromFile("modelos/grass.jpg"), "texture_diffuse", "modelos/grass.jpg" };
	textures.push_back(t2);

	min = max = position;

	//aux
	float delta_h = width / (density - 1);
	float delta_v = height / (density - 1);

	for (int i = 0; i < density; i++)
	{
		for (int j = 0; j < density; j++)
		{
			vec3 point = position + (i * delta_h * direction) + (j * delta_v * v);
			vertices.push_back({ point, u, vec2(i / float(density - 1), j / float(density - 1)) });
			//indices
			if (j != density - 1 && i != density - 1)
			{
				//i, j -> i + 1, j -> i, j + 1
				indices.push_back(j * density + i);
				indices.push_back(j * density + (i + 1));
				indices.push_back((j + 1) * density + i);

				//i+1, j -> i + 1, j + 1, i, j + 1
				indices.push_back(j * density + i + 1);
				indices.push_back((j + 1) * density + i + 1);
				indices.push_back((j + 1) * density + i);
			}

			//actualizar bounds
			if (point.x < min.x) min.x = point.x;
			if (point.y < min.y) min.y = point.y;
			if (point.z < min.z) min.z = point.z;
			if (point.x > max.x) max.x = point.x;
			if (point.y > max.y) max.y = point.y;
			if (point.z > max.z) max.z = point.z;
		}
	}

	mesh = new Mesh(vertices, indices, textures);

	pointsInitialAABB[0] = glm::vec3(min.x, min.y, min.z);
	pointsInitialAABB[1] = glm::vec3(min.x, min.y, max.z);
	pointsInitialAABB[2] = glm::vec3(min.x, max.y, min.z);
	pointsInitialAABB[3] = glm::vec3(min.x, max.y, max.z);
	pointsInitialAABB[4] = glm::vec3(max.x, min.y, min.z);
	pointsInitialAABB[5] = glm::vec3(max.x, min.y, max.z);
	pointsInitialAABB[6] = glm::vec3(max.x, max.y, min.z);
	pointsInitialAABB[7] = glm::vec3(max.x, max.y, max.z);
	calcBounds();
}

vec3 Terrain::getPosition()
{
	return position;
}

vec3 Terrain::getDirection()
{
	return direction;
}

Shader* Terrain::getShaderProgram()
{
	return shaderProgram;
}

mat4 Terrain::getModelMatrix()
{
	return modelMatrix;
}

int Terrain::getDensity()
{
	return density;
}

void Terrain::draw()
{
	shaderProgram->setVec3("lightColor", Settings::getInstance()->getLights()[0]->color);
	shaderProgram->setVec3("lightPos", Settings::getInstance()->getLights()[0]->position);
	shaderProgram->setVec3("viewPos", Settings::getInstance()->getNowCamera()->getPosition());
	mesh->Draw(shaderProgram->ID);
}

Terrain::~Terrain()
{
}