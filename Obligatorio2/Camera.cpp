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

Camera::Camera(vec3 p, float h, float v, float fy, float a, float n, float f, float s, bool pm)
{
	size = s;
	position = p;
	position.y = Settings::getInstance()->getHeightTerrain(position.x, position.z) + size * 0.5;
	mode = WALK;
	fovy = radians(fy);
	aspect = a;
	tnear = n;
	tfar = f;
	perspectiveMode = pm;
	horizontalAngle = clampHorizontal(h);
	verticalAngle = clampVertical(v);

	vec3 front = normalize(spherical_to_cartesian(horizontalAngle, verticalAngle));
	vec3 r = normalize(cross(front, vec3(0.f, 1.f, 0.f)));
	vec3 up = normalize(cross(r, front));

	//matrices de la camara
	view_matrix = lookAt(position, position + front, up);
	projection_matrix = perspective(fovy, aspect, tnear, tfar);

	//calculo del frustum
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
void Camera::setPosition(vec3 pos)
{
	position=pos;
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

	//Si estoy caminando la altura es la del terreno
	if (mode == WALK && !perspectiveMode) position.y = Settings::getInstance()->getHeightTerrain(position.x, position.z) + size * 0.5f;

	//Si estoy caminando y la altura del terreno es -1 (fuera de los limetes) o choco con alguna AABB
	//if ( (mode == WALK && position.y == -1 + size * 0.5f) || Settings::getInstance()->colliding(min_camera, max_camera)) position = last_position;
	//else {
		vec3 target = position + spherical_to_cartesian(horizontalAngle, verticalAngle);
		view_matrix = lookAt(position, target, u);

		//actualizar frustum
		updateFrustum();
	//}
}

void Camera::moveCamera(float h_cant, float v_cant)
{
	//Actualizar valores de los angulos
	horizontalAngle = clampHorizontal(horizontalAngle + h_cant);
	verticalAngle = clampVertical(verticalAngle + v_cant);

	//Sistema de coordenadas de la camara
	vec3 n = normalize(spherical_to_cartesian(horizontalAngle, verticalAngle));
	vec3 r = normalize(cross(n, vec3(0.f, 1.f, 0.f)));
	vec3 u = normalize(cross(r, n));

	vec3 target = position + n;
	view_matrix = lookAt(position, target, u);

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

Camera::~Camera()
{
}

void Camera::updateFrustum()
{
	mat4 m = projection_matrix * view_matrix;

	//Codigo sacado de internet
	frustum[0].normal.x = m[0][3] - m[0][0];
	frustum[0].normal.y = m[1][3] - m[1][0];
	frustum[0].normal.z = m[2][3] - m[2][0];
	frustum[0].d = m[3][3] - m[3][0];
	
	frustum[1].normal.x = m[0][3] + m[0][0];
	frustum[1].normal.y = m[1][3] + m[1][0];
	frustum[1].normal.z = m[2][3] + m[2][0];
	frustum[1].d = m[3][3] + m[3][0];

	frustum[2].normal.x = m[0][3] + m[0][1];
	frustum[2].normal.y = m[1][3] + m[1][1];
	frustum[2].normal.z = m[2][3] + m[2][1];
	frustum[2].d = m[3][3] + m[3][1];

	frustum[3].normal.x = m[0][3] - m[0][1];
	frustum[3].normal.y = m[1][3] - m[1][1];
	frustum[3].normal.z = m[2][3] - m[2][1];
	frustum[3].d = m[3][3] - m[3][1];

	frustum[4].normal.x = m[0][3] - m[0][2];
	frustum[4].normal.y = m[1][3] - m[1][2];
	frustum[4].normal.z = m[2][3] - m[2][2];
	frustum[4].d = m[3][3] - m[3][2];

	frustum[5].normal.x = m[0][3] + m[0][2];
	frustum[5].normal.y = m[1][3] + m[1][2];
	frustum[5].normal.z = m[2][3] + m[2][2];
	frustum[5].d = m[3][3] + m[3][2];

	for (int i = 0; i < 6; i++) frustum[i].normalize();

}

bool Camera::intersectionSphereFrustum(vec3 center, float radio)
{
	for (int i = 0; i < 6; i++)
		if (dot(center, frustum[i].normal) + frustum[i].d + radio < 0)
			return false;
	return true;
}