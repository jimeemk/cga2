#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <map>

#include "Entity.h"

using namespace glm;

class OctreeNode
{
private:
	vec3 point;
	int size;
	OctreeNode* childs[2][2][2];
	std::vector<Entity*> entities;
public:
	OctreeNode();
	OctreeNode(vec3, int);
	bool addObject(Entity*, vec3);
	vec3 getPoint();
	int getSize();
	OctreeNode* getChild(int, int, int);
	~OctreeNode();
};

