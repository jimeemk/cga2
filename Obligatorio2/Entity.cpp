#include "Entity.h"


Entity::Entity()
{
}

Entity::Entity(glm::vec3 pos, glm::vec3 u, glm::vec3 dir, Shader sp)
{
	position = pos;
	direction = dir;
	shaderProgram = sp;
	modelMatrix = glm::mat4(1.0);
	up = u;
}

void Entity::getBounds(glm::vec3& minimum, glm::vec3& maximum)
{
	minimum = min;
	maximum = max;
}

void Entity::calcBounds()
{
    bool firstTime = true;
    for (int i = 0; i < 8; i++)
    {
        glm::vec3 point = pointsInitialAABB[i];
        glm::vec4 aux = glm::vec4(point.x, point.y, point.z, 1.0);
        aux = modelMatrix * aux;
        point = glm::vec3(aux.x / aux.w, aux.y / aux.w, aux.z / aux.w);

        if (firstTime)
        {
            firstTime = false;
            min = point;
            max = point;
        }
        else
        {
            if (point.x < min.x)
                min.x = point.x;
            if (point.y < min.y)
                min.y = point.y;
            if (point.z < min.z)
                min.z = point.z;
            if (point.x > max.x)
                max.x = point.x;
            if (point.y > max.y)
                max.y = point.y;
            if (point.z > max.z)
                max.z = point.z;
        }
    }
    cout << "minAfterRot: " << min.x << " ... " << min.y << " ... " << min.z << "\n";
    cout << "maxAfterRot: " << max.x << " ... " << max.y << " ... " << max.z << "\n";
}

Entity::~Entity()
{
}

