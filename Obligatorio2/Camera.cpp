#include "Camera.h"

vec3 spherical_to_cartesian(float h, float v)
{
	return normalize(vec3(cos(v) * sin(h), sin(v), cos(v) * cos(h)));
}

float clampHorizontal(float h)
{
	while (h < 0) h += 2 * pi<float>();
	while (h > 2 * pi<float>()) h -= 2 * pi<float>();
	return h;
}

float clampVertical(float v)
{
	if (v < -half_pi<float>()) v = -half_pi<float>() + 0.001;
	if (v > half_pi<float>()) v = half_pi<float>() - 0.001;
	return v;
}

Camera::Camera()
{
}

Camera::Camera(vec3 p, float h, float v, float fy, float a, float n, float f, float s)
{
	size = s;
	position = p;
	position.y = Settings::getInstance()->getHeightTerrain(position.x, position.z) + size * 0.5;
	mode = WALK;
	fovy = fy;
	aspect = a;
	tnear = n;
	tfar = f;
	horizontalAngle = clampHorizontal(h);
	verticalAngle = clampVertical(v);

	vec3 target = position + spherical_to_cartesian(horizontalAngle, verticalAngle);

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
	vec3 n = normalize(spherical_to_cartesian(horizontalAngle, verticalAngle));
	vec3 r = normalize(cross(n, vec3(0.f, 1.f, 0.f)));
	vec3 u = normalize(cross(r, n));

	vec3 move_direction = n;
	vec3 right_direction = r;
	vec3 up_direction = vec3(0.f, 1.f, 0.f);

	if (mode == WALK)
	{
		move_direction.y = 0.f;
		move_direction = normalize(move_direction);
		right_direction = normalize(cross(move_direction, vec3(0.f, 1.f, 0.f)));
	}

	vec3 last_position = position;

	switch (d)
	{
	case FRONT:
		position += move_direction * delta;
		break;
	case RIGHT:
		position += right_direction * delta;
		break;
	case LEFT:
		position -= right_direction * delta;
		break;
	case BACK:
		position -= move_direction * delta;
		break;
	case UP:
		position += up_direction * delta;
		break;
	case DOWN:
		position -= up_direction * delta;
		break;
	default:
		break;
	}

	position = Settings::getInstance()->clampToScene(position);
	if (mode == WALK) position.y = Settings::getInstance()->getHeightTerrain(position.x, position.z) + size * 0.5f;
	
	//Si colisono con algo o estoy fuera, no me muevo
	if ( mode == WALK && position.y == -1) position = last_position;
	else {
		vec3 target = position + spherical_to_cartesian(horizontalAngle, verticalAngle);
		view_matrix = lookAt(position, target, u);

		//actualizar frustum
		updateFrustum();
	}
}

void Camera::moveCamera(float h_cant, float v_cant)
{
	//camera coords
	horizontalAngle = clampHorizontal(horizontalAngle + h_cant);
	verticalAngle = clampVertical(verticalAngle + v_cant);
	vec3 target = position + spherical_to_cartesian(horizontalAngle, verticalAngle);
	view_matrix = lookAt(position, target, vec3(0.f, 1.f, 0.f));

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
	vec3 target = spherical_to_cartesian(horizontalAngle, verticalAngle);
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
		frustum[i].d = -dot(frustum[i].normal, points[i]);
	}
}

Camera::~Camera()
{
}

bool Camera::intersectionSphereFrustum(vec3 center, float radio)
{
	for (int i = 0; i < 6; i++)
	{
		if (dot(center, frustum[i].normal) + frustum[i].d + radio < 0) return false;
	}

	return true;
}
