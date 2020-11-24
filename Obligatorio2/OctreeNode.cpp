#include "OctreeNode.h"

OctreeNode::OctreeNode()
{
}

OctreeNode::OctreeNode(vec3 p, float s)
{
	point = p;
	size = s;
	for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) for (int k = 0; k < 2; k++) childs[i][j][k] = NULL;
	entities_ids = std::vector<int>();
}

bool OctreeNode::addEntity(int id, int depth)
{
	vec3 min, max;
	Entity* ent = Settings::getInstance()->getEntities()[id];
	ent->getBounds(min, max);

	//No corresponde agregarlo en este nodo.
	if (min.x > point.x + size || max.x < point.x) return false;
	if (min.y > point.y + size || max.y < point.y) return false;
	if (min.z > point.z + size || max.z < point.z) return false;

	//casos base => altura maxima o unico objeto en nodo
	if (depth == 5 || entities_ids.size() == 0)
	{
		entities_ids.push_back(id);
		return true;
	}

	//recursion

	//saco el que estaba en ese nodo.
	int id2 = entities_ids[0];

	//Intento agregarlos en los 8 hijos.
	for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) for (int k = 0; k < 2; k++)
	{
		bool create_now = false;
		if (childs[i][j][k] == NULL)
		{
			create_now = true;
			float n_size = size * 0.5;
			vec3 n_point = vec3(point.x + i * n_size, point.x + j * n_size, point.x + k * n_size);
			childs[i][j][k] = new OctreeNode(n_point, n_size);
		}

		bool use_node = childs[i][j][k]->addEntity(id, depth + 1) || childs[i][j][k]->addEntity(id2, depth + 1);

		//Si no agrego ninguno de los dos objetos y se acaba de crear elimino ese hijo.
		if (!use_node && create_now)
		{
			delete childs[i][j][k];
			childs[i][j][k] = NULL;
		}
	}
	return true;
}

vec3 OctreeNode::getPoint()
{
	return point;
}

float OctreeNode::getSize()
{
	return size;
}

OctreeNode* OctreeNode::getChild(int i, int j, int k)
{
	return childs[i][j][k];
}

void OctreeNode::draw()
{
	bool h_childs = false;
	for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) for (int k = 0; k < 2; k++)
	{
		if (childs[i][j][k] != NULL)
		{
			childs[i][j][k]->draw();
			h_childs = true;
		}
	}

	if (h_childs) return;

	vec3 o = point;
	float vertices[] = {
		 o.x, o.y, o.z,  0.0f,  0.0f, -1.0f,
		 o.x + size, o.y, o.z,  0.0f,  0.0f, -1.0f,
		 o.x + size,  o.y + size, o.z,  0.0f,  0.0f, -1.0f,
		 o.x + size,  o.y + size, o.z,  0.0f,  0.0f, -1.0f,
		 o.x,  o.y + size, o.z,  0.0f,  0.0f, -1.0f,
		 o.x, o.y, o.z,  0.0f,  0.0f, -1.0f,

		 o.x, o.y,  o.z + size,  0.0f,  0.0f,  1.0f,
		 o.x + size, o.y,  o.z + size,  0.0f,  0.0f,  1.0f,
		 o.x + size,  o.y + size,  o.z + size,  0.0f,  0.0f,  1.0f,
		 o.x + size,  o.y + size,  o.z + size,  0.0f,  0.0f,  1.0f,
		 o.x,  o.y + size,  o.z + size,  0.0f,  0.0f,  1.0f,
		 o.x, o.y,  o.z + size,  0.0f,  0.0f,  1.0f,

		 o.x,  o.y + size,  o.z + size, -1.0f,  0.0f,  0.0f,
		 o.x,  o.y + size, o.z, -1.0f,  0.0f,  0.0f,
		 o.x, o.y, o.z, -1.0f,  0.0f,  0.0f,
		 o.x, o.y, o.z, -1.0f,  0.0f,  0.0f,
		 o.x, o.y, o.z + size, -1.0f,  0.0f,  0.0f,
		 o.x,  o.y + size,  o.z + size, -1.0f,  0.0f,  0.0f,

		 o.x + size, o.y + size,  o.z + size,  1.0f,  0.0f,  0.0f,
		 o.x + size, o.y + size, o.z,  1.0f,  0.0f,  0.0f,
		 o.x + size, o.y, o.z,  1.0f,  0.0f,  0.0f,
		 o.x + size, o.y, o.z,  1.0f,  0.0f,  0.0f,
		 o.x + size, o.y,  o.z + size,  1.0f,  0.0f,  0.0f,
		 o.x + size, o.y + size,  o.z + size,  1.0f,  0.0f,  0.0f,

		 o.x, o.y, o.z,  0.0f, -1.0f,  0.0f,
		 o.x + size, o.y, o.z,  0.0f, -1.0f,  0.0f,
		 o.x + size, o.y,  o.z + size,  0.0f, -1.0f,  0.0f,
		 o.x + size, o.y, o.z + size,  0.0f, -1.0f,  0.0f,
		 o.x, o.y,  o.z + size,  0.0f, -1.0f,  0.0f,
		 o.x, o.y, o.z,  0.0f, -1.0f,  0.0f,

		 o.x,  o.y + size, o.z,  0.0f,  1.0f,  0.0f,
		 o.x + size,  o.y + size, o.z,  0.0f,  1.0f,  0.0f,
		 o.x + size,  o.y + size,  o.z + size,  0.0f,  1.0f,  0.0f,
		 o.x + size,  o.y + size,  o.z + size,  0.0f,  1.0f,  0.0f,
		 o.x,  o.y + size, o.z + size,  0.0f,  1.0f,  0.0f,
		 o.x,  o.y + size, o.z,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, octreeNodeVAO;
	glGenVertexArrays(1, &octreeNodeVAO);
	glBindVertexArray(octreeNodeVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(octreeNodeVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

OctreeNode::~OctreeNode()
{
}