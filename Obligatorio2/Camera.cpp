#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(vec3 p, vec3 t, float fy, float a, float n, float f)
{
	position = p;
	target = t;
	mode = WALK;
	fovy = fy;
	aspect = a;
	tnear = n;
	tfar = f;

	//matrices de la camara
	view_matrix = lookAt(position, target, vec3(0.f, 1.f, 0.f));
	projection_matrix = perspective(fovy, aspect, tnear, tfar);

	//calculo del frustum
	//near, far, left, right, bottom, up
	updateFrustum();
}

void Camera::changeMode()
{
	if (mode == WALK)
	{
		mode = FLY;
		std::cout << "fly" << std::endl;
	}
	else 
	{
		mode = WALK;
		std::cout << "walk" << std::endl;
	}
}

vec3 Camera::getPosition()
{
	return position;
}

void Camera::updatePosition(float delta, movement_direction d)
{
	//camera coords
	vec3 n = normalize(target - position);
	vec3 r = normalize(cross(n, vec3(0.f, 1.f, 0.f)));
	vec3 u = normalize(cross(r, n));

	vec3 move_direction = n;
	vec3 right_direction = r;
	vec3 up_direction = vec3(0.f, 1.f, 0.f);

	if (mode == WALK)
	{
		if (n.y == 1.f || n.y == -1.f) n.x = 0.1;
		move_direction = normalize(vec3(n.x, 0.f, n.z));
		right_direction = normalize(cross(move_direction, vec3(0.f, 1.f, 0.f)));
		up_direction = vec3(0.f);
	}

	switch (d)
	{
	case FRONT:
		position += move_direction * delta;
		target += move_direction * delta;
		break;
	case RIGHT:
		position += right_direction * delta;
		target += right_direction * delta;
		break;
	case LEFT:
		position -= right_direction * delta;
		target -= right_direction * delta;
		break;
	case BACK:
		position -= move_direction * delta;
		target -= move_direction * delta;
		break;
	case UP:
		position += up_direction * delta;
		target += up_direction * delta;
		break;
	case DOWN:
		position -= up_direction * delta;
		target -= up_direction * delta;
		break;
	default:
		break;
	}
	view_matrix = lookAt(position, target, u);

	//actualizar frustum
	updateFrustum();
}

void Camera::moveCamera(float h_cant, float v_cant)
{
	//camera coords
	vec3 dir = normalize(target - position);
	vec3 right = normalize(cross(vec3(0.f, 1.f, 0.f), dir));
	vec3 up = normalize(cross(dir, right));

	//matriz de rotacion
	mat4 rot_mat = rotate(mat4(1.f), h_cant, up);
	right = rot_mat * vec4(right, 0.f);
	rot_mat = rotate(rot_mat, v_cant, right);
	dir = rot_mat * vec4(dir, 0.f);
	up = rot_mat * vec4(up, 0.f);

	//actualizar target y view_matrix
	target = position + dir;
	view_matrix = lookAt(position, target, up);

	//actualizar frustum
	updateFrustum();
}

mat4 Camera::getViewMatrix()
{
	return view_matrix;
}

mat4 Camera::getProjectionMatrix()
{
	return projection_matrix;
}

void Camera::getFrustum(plane ret[6])
{
	for (int i = 0; i < 6; i++) ret[i] = frustum[i];
}

void Camera::updateFrustum()
{
	vec3 front = normalize(target - position);
	vec3 right = normalize(cross(front, vec3(0.f, 1.f, 0.f)));
	vec3 up = normalize(cross(right, front));

	float fovh = fovy * aspect;

	vec3 points[6] = { position + tnear * front , position + tfar * front, position, position, position, position };
	vec3 normals[6] = {
		front,
		-front,
		rotate(front, radians(fovh - 90), up),
		rotate(front, radians(90 - fovh), up),
		rotate(front, radians(fovy - 90), right),
		rotate(front, radians(90 - fovy), right) };

	for (int i = 0; i < 6; i++)
	{
		frustum[i].normal = normalize(normals[i]);
		frustum[i].point = points[i];
	}
}

Camera::~Camera()
{
}