#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <map>

#include "Settings.h"

using namespace glm;

class OctreeNode
{
private:
	vec3 point;
	float size;
	OctreeNode* childs[2][2][2];
	std::vector<int> entities_ids;
public:
	OctreeNode();
	OctreeNode(vec3, float);
	bool addEntity(int, int);
	vec3 getPoint();
	float getSize();
	OctreeNode* getChild(int, int, int);
	void draw();
	~OctreeNode();
};

