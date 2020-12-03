#include "Terrain.h"


Terrain::Terrain()
{
}

Terrain::Terrain(vec3 pos, vec3 u, vec3 dir, Shader* sp, int den, float width, float height, float t, vector<Texture> text, std::string hm) : Entity(pos, normalize(u), normalize(dir), sp)
{
	density = den;
	this->width = width;
	this->height = height;
	texture = text;
	heightMap = hm;
	top = t;
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
	min = max = position;
	FIBITMAP* imagen=nullptr;
	unsigned w = 0;
	unsigned h = 0;
	
	string filename = heightMap;
	imagen = FreeImage_Load(
	FreeImage_GetFileType(filename.c_str(), 0),
	filename.c_str());
	w= FreeImage_GetWidth(imagen)-1;
	h= FreeImage_GetHeight(imagen)-1;

	
	//aux
	float delta_h = width / (density - 1);
	float delta_v = height / (density - 1);

	for (int i = 0; i < density; i++)
	{
		for (int j = 0; j < density; j++)
		{
			vec3 point = position + (i * delta_h * direction) + (j * delta_v * v);
			vec2 tc = vec2(i / float(density - 1), j / float(density - 1));
			RGBQUAD color; 
			bool col=FreeImage_GetPixelColor(imagen, (int)(tc.x*w), (int)(tc.y*h), &color);
			if (col)
			{
				point.y = point.y + (color.rgbRed / 255.0)*top;//Como es siempre gris, miro solo el primer color
			}

			vertices.push_back({ point, u, tc });
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
	FreeImage_Unload(imagen);
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
	shaderProgram->setFloat("top", top);
	shaderProgram->setVec3("lightColor", Settings::getInstance()->getLights()[0]->color);
	shaderProgram->setVec3("lightPos", Settings::getInstance()->getLights()[0]->position);
	shaderProgram->setVec3("viewPos", Settings::getInstance()->getNowCamera()->getPosition());
	mesh->Draw(shaderProgram->ID);
}

Terrain::~Terrain()
{
}

FIBITMAP* Terrain::getHeightFromMap(const char* path) {
	string filename = string(path);
	FIBITMAP* bitmap = FreeImage_Load(
		FreeImage_GetFileType(filename.c_str(), 0),
		filename.c_str());
	unsigned width = FreeImage_GetWidth(bitmap);
	unsigned height = FreeImage_GetHeight(bitmap);
	int bpp = FreeImage_GetBPP(bitmap);

	FIBITMAP* bitmap2 = FreeImage_Allocate(width, height, bpp);
	return bitmap2;
}



void Terrain::aumentarX() //r
{

}

void Terrain::disminuirX() //f
{

}

void Terrain::aumentarY() //t
{

}

void Terrain::disminuirY()//g
{

}

void Terrain::aumentarZ()//y
{

}

void Terrain::disminuirZ() //h
{

}

void Terrain::rotarMasX() //u
{

}

void Terrain::rotarMenosX()//j
{

}

void Terrain::rotarMasY()//i
{

}

void Terrain::rotarMenosY() //k
{

}

void Terrain::rotarMasZ()//o
{

}

void Terrain::rotarMenosZ()//l
{

}

void Terrain::escMasX()//1
{

}

void Terrain::escMenosX()//2
{

}

void Terrain::escMasY()//3
{

}

void Terrain::escMenosY()//4
{

}

void Terrain::escMasZ()//5
{

}

void Terrain::escMenosZ()//6
{

}

void Terrain::guardarEntity()//7
{
	cout << "GUARDAR:";
}

bool Terrain::intersectionBoxBounds(vec3 min, vec3 max)
{
	return false;
}