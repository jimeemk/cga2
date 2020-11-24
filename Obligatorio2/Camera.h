#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>

#include <iostream>
#include "Entity.h"

#define _USE_MATH_DEFINES

using namespace glm;

enum camera_mode {FLY, WALK};
enum movement_direction {FRONT, RIGHT, LEFT, BACK, UP, DOWN};

struct plane {
	vec3 normal;
	float d;
};

class Camera
{
public:
	Camera();
	Camera(vec3, float, float, float, float, float, float); //no recibe modo de la camara, default = walk
	vec3 getPosition();
	mat4 getViewMatrix();
	mat4 getProjectionMatrix();
	void getFrustum(plane[6]);
	void changeMode();
	//delta, direction
	void updatePosition(float, movement_direction);
	void updateFrustum();
	//h_cant, v_cant
	void moveCamera(float, float);
	bool intersectionSphereFrustum(vec3, float);
	~Camera();

private:
	vec3 position;
	float horizontalAngle;
	float verticalAngle;
	camera_mode mode;
	mat4 view_matrix;
	mat4 projection_matrix;
	plane frustum[6];
	float fovy;
	float aspect;
	float tnear;
	float tfar;
};