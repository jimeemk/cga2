#include "OctreeNode.h"

OctreeNode::OctreeNode()
{
}

OctreeNode::OctreeNode(vec3 p, int s)
{
	point = p;
	size = s;
	for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) for (int k = 0; k < 2; k++) childs[i][j][k] = NULL;
	entities = std::vector<Entity*>();
}

bool OctreeNode::addObject(Entity* ent, vec3 p)
{
	vec3 min, max;
	ent->getBounds(min, max);
	//Traslado respecto al origen del octree
	min -= p;
	max -= p;

	//No corresponde agregarlo en este nodo.
	if (min.x > point.x + size || max.x < point.x) return false;
	if (min.y > point.y + size || max.y < point.y) return false;
	if (min.z > point.z + size || max.z < point.z) return false;

	//casos base => altura maxima o unico objeto en nodo
	if (size == 1 || entities.size() == 0)
	{
		entities.push_back(ent);
		return true;
	}

	//recursion

	//saco el que estaba en ese nodo.
	Entity* ent2 = entities[0];

	//Intento agregarlos en los 8 hijos.
	for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) for (int k = 0; k < 2; k++)
	{
		bool create_now = false;
		if (childs[i][j][k] == NULL)
		{
			create_now = true;
			int n_size = size / 2;
			vec3 n_point = vec3(point.x + i * n_size, point.x + j * n_size, point.x + k * n_size);
			childs[i][j][k] = new OctreeNode(n_point, n_size);
		}

		bool use_node = childs[i][j][k]->addObject(ent, p) || childs[i][j][k]->addObject(ent2, p);

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

int OctreeNode::getSize()
{
	return size;
}

OctreeNode* OctreeNode::getChild(int i, int j, int k)
{
	return childs[i][j][k];
}

OctreeNode::~OctreeNode()
{
}