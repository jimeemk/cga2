#pragma once

#include <vector>
#include "Entity.h"
#include "OctreeNode.h"
#include <iostream>
#include <fstream>

using namespace glm;

//forward declaration que compile
class OctreeNode;
class Entity;

class Octree 
{
private:
	vec3 point;
	float size;
	OctreeNode* root;
public:
	Octree();
	~Octree();
	Octree(vec3, float);
	bool addEntity(int);
	void draw();
};

