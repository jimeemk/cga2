#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>

#include <iostream>

#define _USE_MATH_DEFINES

using namespace glm;

enum camera_mode {FLY, WALK};
enum movement_direction {FRONT, RIGHT, LEFT, BACK, UP, DOWN};

class Camera
{
public:
	Camera();
	Camera(vec3, vec3); //no recibe modo de la camara, default = walk
	vec3 getPosition();
	mat4 getViewMatrix();
	void changeMode();
	//delta, direction
	void updatePosition(float, movement_direction);
	//h_cant, v_cant
	void moveCamera(float, float);
	~Camera();

private:
	vec3 position;
	vec3 target;
	camera_mode mode;
	mat4 view_matrix;
};