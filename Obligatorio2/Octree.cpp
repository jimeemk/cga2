#include "Octree.h"

Octree::Octree()
{
}

Octree::Octree(vec3 p, float s)
{
	int i_size = 1;
	//Primer potencia de 2 que sea mas grande que es size
	while (i_size < s) i_size = 2 * i_size;
	point = p;
	size = s;
	root = new OctreeNode(vec3(0.f), i_size);
}

bool Octree::addObject(Entity* ent)
{
	return root->addObject(ent, point);
}

//Funcion para probar
void guardarPuntosNodos(OctreeNode* node, std::vector<vec3>* points)
{
	vec3 a = node->getPoint();
	int s = node->getSize();
	points->push_back(a);
	points->push_back(vec3(a.x, a.y, a.z + s));
	points->push_back(vec3(a.x, a.y + s,  a.z));
	points->push_back(vec3(a.x, a.y + s, a.z + s));
	points->push_back(vec3(a.x + s, a.y, a.z));
	points->push_back(vec3(a.x + s, a.y, a.z + s));
	points->push_back(vec3(a.x + s, a.y + s, a.z));
	points->push_back(vec3(a.x + s, a.y + s, a.z + s));

	for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) for (int k = 0; k < 2; k++)
	{
		if (node->getChild(i, j, k) != NULL) guardarPuntosNodos(node->getChild(i, j, k), points);
	}
}

void Octree::guardarPuntos()
{
	std::ofstream file;
	file.open("puntos.txt");
	std::vector<vec3>* puntos = new std::vector<vec3>();
	guardarPuntosNodos(root, puntos);
	std::vector<vec3>::iterator it;
	std::cout << "size: " << puntos->size() << std::endl;
	for (it = puntos->begin(); it != puntos->end(); ++it)
	{
		file << it->x / 100.f << "/" << it->y / 100.f << "/" << it->z / 100.f << "/" << 1.0 << "/" << 1.0 << "/" << 1.0 << "/\n";
	}

	file.close();
}

Octree::~Octree()
{
}
