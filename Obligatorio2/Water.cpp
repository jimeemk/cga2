#include "Water.h"

float clampTwoPI(float h)
{
	while (h < 0) h += 2 * pi<float>();
	while (h > 2 * pi<float>()) h -= 2 * pi<float>();
	return h;
}

Water::Water()
{
}

Water::Water(vec3 pos, vec3 u, vec3 dir, Shader* sp, int den, float width, float height, std::string text) : Entity(pos, normalize(u), normalize(dir), sp)
{
	density = den;
	this->width = width;
	this->height = height;
	texture = text;
	modelMatrix = mat4(1.f);
	last_time = SDL_GetTicks();
	c = 0;
	setupWater();
}

void Water::setupWater()
{
	vec3 v = normalize(cross(up, direction));
	vec3 u = normalize(cross(direction, v));
	//vectores
	std::vector<Vertex> vertices;
	std::vector<unsigned int>indices;
	std::vector<Texture>textures;
	Texture t = { TextureFromFile(texture.c_str()), "texture_height", texture };
	textures.push_back(t);

	min = max = position;

	//aux
	float delta_h = width / (density - 1);
	float delta_v = height / (density - 1);

	for (int i = 0; i < density; i++)
	{
		for (int j = 0; j < density; j++)
		{
			vec3 point = position + (i * delta_h * direction) + (j * delta_v * v);
			vertices.push_back({point, u, vec2(i / float(density - 1), j / float(density - 1))});

			//indices
			if (j != density - 1 && i != density - 1)
			{
				//i, j -> i + 1, j -> i, j + 1
				indices.push_back(j * density + i);
				indices.push_back(j * density + (i + 1));
				indices.push_back((j + 1) * density + i);

				//i+1, j -> i + 1, j + 1, i, j + 1
				indices.push_back(j * density + i + 1);
				indices.push_back((j + 1)* density + i + 1);
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

vec3 Water::getPosition()
{
	return position;
}

vec3 Water::getDirection()
{
	return direction;
}

Shader* Water::getShaderProgram()
{
	return shaderProgram;
}

mat4 Water::getModelMatrix()
{
	return modelMatrix;
}

int Water::getDensity()
{
	return density;
}

void Water::draw()
{
	int now = SDL_GetTicks();
	float delta_time = (now - last_time) / 1000.f;
	//last_time = now;
	shaderProgram->setFloat("passedTime", (now - last_time) / 1000.f);
	shaderProgram->setVec3("lightColor", Settings::getInstance()->getLights()[0]->color);
	shaderProgram->setVec3("lightPos", Settings::getInstance()->getLights()[0]->position);
	shaderProgram->setVec3("viewPos", Settings::getInstance()->getNowCamera()->getPosition());
	mesh->Draw(shaderProgram->ID);
}

Water::~Water()
{
}