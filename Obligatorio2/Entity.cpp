#include "Entity.h"

Entity::Entity()
{
}

Entity::Entity(glm::vec3 pos, glm::vec3 dir, float sca, GLuint sp)
{
	position = pos;
	direction = dir;
	scale = sca;
	shaderProgram = sp;
	modelMatrix = glm::mat4(1.0);
}

Entity::~Entity()
{
}

