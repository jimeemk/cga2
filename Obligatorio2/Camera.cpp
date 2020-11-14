#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(vec3 p, vec3 t)
{
	position = p;
	target = t;
	view_matrix = lookAt(position, target, vec3(0.f, 1.f, 0.f));
	mode = walk;
}

void Camera::changeMode()
{
	if (mode == walk)
	{
		mode = fly;
		std::cout << 'fly' << std::endl;
	}
	else 
	{
		mode = walk;
		std::cout << 'walk' << std::endl;
	}
}

vec3 Camera::getPosition()
{
	return position;
}

void Camera::updatePosition(float delta, movement_direction d)
{
	//camera coords
	vec3 dir = normalize(target - position);
	vec3 r = normalize(cross(dir, vec3(0.f, 1.f, 0.f)));
	vec3 u = normalize(cross(r, dir));

	switch (d)
	{
	case front:
		position += dir * delta;
		target += dir * delta;
		break;
	case right:
		position += r * delta;
		target += r * delta;
		break;
	case left:
		position -= r * delta;
		target -= r * delta;
		break;
	case back:
		position -= dir * delta;
		target -= dir * delta;
		break;
	case up:
		position += u * delta;
		target += u * delta;
		break;
	case down:
		position -= u * delta;
		target -= u * delta;
		break;
	default:
		break;
	}
	view_matrix = lookAt(position, target, u);
}

void Camera::moveCamera(float h_cant, float v_cant)
{
	//position, target, up
	//camera coords
	vec3 dir = normalize(target - position);
	vec3 right = normalize(cross(vec3(0.f, 1.f, 0.f), dir));
	vec3 up = normalize(cross(dir, right));

	mat4 rot_mat = rotate(mat4(1.f), -h_cant, up);
	right = rot_mat * vec4(right, 0.f);
	rot_mat = rotate(rot_mat, -v_cant, right);
	dir = rot_mat * vec4(dir, 0.f);
	up = rot_mat * vec4(up, 0.f);

	target = position + dir;
	view_matrix = lookAt(position, target, up);
}

mat4 Camera::getViewMatrix()
{
	return view_matrix;
}

Camera::~Camera()
{
}