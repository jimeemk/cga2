#include "Plane.h"


Plane::Plane(string tex, int texx, int texy, glm::vec3 sca, glm::vec3 pos, glm::vec3 u, glm::vec3 dir, Shader* sh):Entity(pos,u,dir,sh)
{
	textureRepetitionX = texx;
	textureRepetitionY = texy;
	scale = sca;
	texture = tex;
	initPlane();
}

Plane::~Plane()
{
}

glm::vec3 Plane::getPosition()
{
	return position;
}

glm::vec3 Plane::getDirection()
{
	return direction;
}

Shader* Plane::getShaderProgram()
{
	return shaderProgram;
}

glm::mat4 Plane::getModelMatrix()
{
	return modelMatrix;
}

void Plane::draw()
{
	shaderProgram->setVec3("lightColor", Settings::getInstance()->getLights()[0]->color);
	shaderProgram->setVec3("lightPos", Settings::getInstance()->getLights()[0]->position);
	shaderProgram->setVec3("viewPos", Settings::getInstance()->getNowCamera()->getPosition());
	mesh->Draw(shaderProgram->ID);
}

void Plane::initPlane()
{
	glm::vec3 v1=position;
	glm::vec3 v2=position+normalize(up);
	glm::vec3 v3=position+normalize(glm::rotate(normalize(up),-glm::half_pi<float>(),normalize(direction)));
	glm::vec3 v4=v2+(v3-v1);
	vector<Vertex>vertices;
	vertices.push_back({v1,direction,glm::vec2(0,0)}); //izq abajo
	vertices.push_back({ v2,direction,glm::vec2(0,textureRepetitionY)}); //izq arriba
	vertices.push_back({ v3,direction,glm::vec2(textureRepetitionX,0)});// der abajo
	vertices.push_back({ v4,direction,glm::vec2(textureRepetitionX,textureRepetitionY)});// der arriba
	vector<unsigned int>indices;
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	vector<Texture>textures;
	Texture t = { TextureFromFile(texture.c_str()),"texture_diffuse", texture};
	textures.push_back(t);
	mesh = new Mesh(vertices, indices, textures);
	//La siguiente parte es para el bound
	bool firstTime=true;
	for (int j = 0; j < vertices.size(); j++)
	{
		glm::vec3 point = vertices.at(j).Position;
		if (firstTime)
		{
			firstTime = false;
			min = point;
			max = point;
		}
		else
		{
			if (point.x < min.x)
				min.x = point.x;
			if (point.y < min.y)
				min.y = point.y;
			if (point.z < min.z)
				min.z = point.z;
			if (point.x > max.x)
				max.x = point.x;
			if (point.y > max.y)
				max.y = point.y;
			if (point.z > max.z)
				max.z = point.z;
		}
	}
	glm::vec3 centro;
	centro.x = (min.x + max.x) / 2;
	centro.y = (min.y + max.y) / 2;
	centro.z = (min.z + max.z) / 2;

	pointsInitialAABB[0] = glm::vec3(min.x, min.y, min.z);
	pointsInitialAABB[1] = glm::vec3(min.x, min.y, max.z);
	pointsInitialAABB[2] = glm::vec3(min.x, max.y, min.z);
	pointsInitialAABB[3] = glm::vec3(min.x, max.y, max.z);
	pointsInitialAABB[4] = glm::vec3(max.x, min.y, min.z);
	pointsInitialAABB[5] = glm::vec3(max.x, min.y, max.z);
	pointsInitialAABB[6] = glm::vec3(max.x, max.y, min.z);
	pointsInitialAABB[7] = glm::vec3(max.x, max.y, max.z);

	glm::vec3 auxVector = v1;
	auxVector= auxVector+ glm::vec3(-centro.x, -centro.y, -centro.z);
	auxVector = glm::vec3(auxVector.x * scale.x, auxVector.y * scale.y, auxVector.z * scale.z);
	auxVector = v1 - auxVector;//Calculo donde quedo v1 luego del escalamiento para retornarlo al mismo lugar

	modelMatrix = glm::translate(modelMatrix, glm::vec3(auxVector.x, auxVector.y, auxVector.z));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale.x, scale.y, scale.z)); //Inicialmente es de lado 1
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-centro.x, -centro.y, -centro.z));

	calcBounds();
	glm::vec3 maxAux;
	glm::vec3 minAux;
	getBounds(minAux, maxAux);//Esto no va aca, es para probar

}

