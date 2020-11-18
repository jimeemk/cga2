#pragma once

#include <vector>
#include "Entity.h"
#include "OctreeNode.h"

#include <iostream>
#include <fstream>

using namespace glm;

class Octree 
{
private:
	vec3 point;
	float size;
	OctreeNode* root;
	std::vector<Entity*> entities;
public:
	Octree();
	~Octree();
	Octree(vec3, float);
	bool addObject(Entity*);
	void guardarPuntos();
};

