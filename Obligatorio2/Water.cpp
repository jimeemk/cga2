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

Water::Water(vec3 pos, vec3 u, vec3 dir, Shader* sp, int den, float width, float height, std::vector<Texture> text) : Entity(pos, normalize(u), normalize(dir), sp)
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

	min = max = position;

	//aux
	float delta_h = width / (density - 1);
	float delta_v = height / (density - 1);

	for (int i = 0; i < density; i++)
	{
		for (int j = 0; j < density; j++)
		{
			vec3 point = position + (i * delta_h * direction) + (j * delta_v * v);
			vertices.push_back({ point, u, vec2(point.x / width, point.z / height) });

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

	mesh = new Mesh(vertices, indices, texture);

	pointsInitialAABB[0] = glm::vec3(min.x, min.y, min.z);
	pointsInitialAABB[1] = glm::vec3(min.x, min.y, max.z);
	pointsInitialAABB[2] = glm::vec3(min.x, max.y, min.z);
	pointsInitialAABB[3] = glm::vec3(min.x, max.y, max.z);
	pointsInitialAABB[4] = glm::vec3(max.x, min.y, min.z);
	pointsInitialAABB[5] = glm::vec3(max.x, min.y, max.z);
	pointsInitialAABB[6] = glm::vec3(max.x, max.y, min.z);
	pointsInitialAABB[7] = glm::vec3(max.x, max.y, max.z);
	calcBounds();

	//Waves
	float overallSteepness = 0.2f;

	// Wave One
	waveParameters[0].x = 19.0f;
	waveParameters[0].y = 1.7f;
	waveParameters[0].z = 12.0f;
	waveParameters[0].w = 0.8f;
	waveDirections[0].x = +1.0f;
	waveDirections[0].y = +1.0f;

	// Wave Two
	waveParameters[1].x = 7.0f; //speed
	waveParameters[1].y = 0.6f; //amplitude
	waveParameters[1].z = 7.0f; //wavelength
	waveParameters[1].w = 0.8f;
	waveDirections[1].x = +1.0f;
	waveDirections[1].y = +0.0f;

	// Wave Three
	waveParameters[2].x = 6.f;
	waveParameters[2].y = 0.15f;
	waveParameters[2].z = 2.0f;
	waveParameters[2].w = 0.8f;
	waveDirections[2].x = -0.1f;
	waveDirections[2].y = -0.2f;

	// Wave Four
	waveParameters[3].x = 8.f;
	waveParameters[3].y = 0.08f;
	waveParameters[3].z = 1.0f;
	waveParameters[3].w = 0.8f;
	waveDirections[3].x = -0.2f;
	waveDirections[3].y = -0.1f;
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
	Settings::getInstance()->SetLightsToShader(shaderProgram);
	shaderProgram->setInt("skybox", 0);
	shaderProgram->setMat4("worldToUnitMatrix", Settings::getInstance()->getWorldToUnitMatrix());
	shaderProgram->setVec3("viewPos", Settings::getInstance()->getNowCamera()->getPosition());

	//cargar waves
	setWavesValues();
	
	mesh->Draw(shaderProgram->ID);
}

void Water::setWavesValues()
{
	//Wave0
	shaderProgram->setVec4("waveParameters[0]", waveParameters[0]);
	shaderProgram->setVec2("waveDirections[0]", waveDirections[0]);

	//Wave1
	shaderProgram->setVec4("waveParameters[1]", waveParameters[1]);
	shaderProgram->setVec2("waveDirections[1]", waveDirections[1]);

	//Wave2
	shaderProgram->setVec4("waveParameters[2]", waveParameters[2]);
	shaderProgram->setVec2("waveDirections[2]", waveDirections[2]);

	//Wave3
	shaderProgram->setVec4("waveParameters[3]", waveParameters[3]);
	shaderProgram->setVec2("waveDirections[3]", waveDirections[3]);
}

Water::~Water()
{
}

void Water::aumentarX() //r
{

}

void Water::disminuirX() //f
{

}

void Water::aumentarY() //t
{

}

void Water::disminuirY()//g
{

}

void Water::aumentarZ()//y
{

}

void Water::disminuirZ() //h
{

}

void Water::rotarMasX() //u
{

}

void Water::rotarMenosX()//j
{

}

void Water::rotarMasY()//i
{

}

void Water::rotarMenosY() //k
{

}

void Water::rotarMasZ()//o
{

}

void Water::rotarMenosZ()//l
{

}

void Water::escMasX()//1
{

}

void Water::escMenosX()//2
{

}

void Water::escMasY()//3
{

}

void Water::escMenosY()//4
{

}

void Water::escMasZ()//5
{

}

void Water::escMenosZ()//6
{

}

void Water::guardarEntity()//7
{
	cout << "GUARDAR:";
}

bool Water::intersectionBoxBounds(vec3 min, vec3 max)
{
	return false;
}