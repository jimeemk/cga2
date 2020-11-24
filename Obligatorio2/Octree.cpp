#include "Octree.h"

Octree::Octree()
{
}

Octree::Octree(vec3 p, float s)
{
	//darle marge al punto que me pasan
	point = p + vec3(-1.f);
	size = s + 2;
	root = new OctreeNode(point, size);
}

bool Octree::addEntity(int id)
{
	return root->addEntity(id, 0);
}

void Octree::draw()
{
	root->draw();
}

Octree::~Octree()
{
}
