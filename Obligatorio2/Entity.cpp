#include "Entity.h"

bool overlap(float a_min, float a_max, float b_min, float b_max)
{
	if (a_min > b_max || b_min > a_max) return false;
	else return true;
}

Entity::Entity()
{
}

Entity::Entity(glm::vec3 pos, glm::vec3 u, glm::vec3 dir, Shader* sp)
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
}

void Entity::getSphericalBounds(glm::vec3& center, float& rad)
{
    center = (max + min) * 0.5f;
    rad = distance(center, min);
}

Entity::~Entity()
{
}

unsigned int Entity::TextureFromFile(const char* path) {
    string filename = string(path);
    FIBITMAP* bitmap = FreeImage_Load(
        FreeImage_GetFileType(filename.c_str(), 0),
        filename.c_str());

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
    int nWidth = FreeImage_GetWidth(pImage);
    int nHeight = FreeImage_GetHeight(pImage);

    if (pImage)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
            0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        FreeImage_Unload(pImage);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        FreeImage_Unload(pImage);
    }

    return textureID;
}

void Entity::drawBounds()
{
	float vertices[] = {
		 min.x, min.y, min.z,  0.0f,  0.0f, -1.0f,
		 max.x, min.y, min.z,  0.0f,  0.0f, -1.0f,
		 max.x, max.y, min.z,  0.0f,  0.0f, -1.0f,
		 max.x, max.y, min.z,  0.0f,  0.0f, -1.0f,
		 min.x, max.y, min.z,  0.0f,  0.0f, -1.0f,
		 min.x, min.y, min.z,  0.0f,  0.0f, -1.0f,

		min.x, min.y,  max.z,  0.0f,  0.0f,  1.0f,
		max.x, min.y,  max.z,  0.0f,  0.0f,  1.0f,
		max.x, max.y,  max.z,  0.0f,  0.0f,  1.0f,
		max.x, max.y,  max.z,  0.0f,  0.0f,  1.0f,
		min.x, max.y,  max.z,  0.0f,  0.0f,  1.0f,
		min.x, min.y,  max.z,  0.0f,  0.0f,  1.0f,

		min.x, max.y, max.z, -1.0f,  0.0f,  0.0f,
		min.x, max.y, min.z, -1.0f,  0.0f,  0.0f,
		min.x, min.y, min.z, -1.0f,  0.0f,  0.0f,
		min.x, min.y, min.z, -1.0f,  0.0f,  0.0f,
		min.x, min.y, max.z, -1.0f,  0.0f,  0.0f,
		min.x, max.y, max.z, -1.0f,  0.0f,  0.0f,

		max.x, max.y, max.z,  1.0f,  0.0f,  0.0f,
		max.x, max.y, min.z,  1.0f,  0.0f,  0.0f,
		max.x, min.y, min.z,  1.0f,  0.0f,  0.0f,
		max.x, min.y, min.z,  1.0f,  0.0f,  0.0f,
		max.x, min.y, max.z,  1.0f,  0.0f,  0.0f,
		max.x, max.y, max.z,  1.0f,  0.0f,  0.0f,

		min.x, min.y, min.z,  0.0f, -1.0f,  0.0f,
		 max.x, min.y, min.z,  0.0f, -1.0f,  0.0f,
		 max.x, min.y,  max.z,  0.0f, -1.0f,  0.0f,
		 max.x, min.y,  max.z,  0.0f, -1.0f,  0.0f,
		min.x, min.y,  max.z,  0.0f, -1.0f,  0.0f,
		min.x, min.y, min.z,  0.0f, -1.0f,  0.0f,

		min.x,  max.y, min.z,  0.0f,  1.0f,  0.0f,
		 max.x,  max.y, min.z,  0.0f,  1.0f,  0.0f,
		 max.x,  max.y,  max.z,  0.0f,  1.0f,  0.0f,
		 max.x,  max.y,  max.z,  0.0f,  1.0f,  0.0f,
		min.x,  max.y,  max.z,  0.0f,  1.0f,  0.0f,
		min.x,  max.y, min.z,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, lightCubeVAO;

	/* Allocate and assign a VAO to our handle */
	glGenVertexArrays(1, &lightCubeVAO);
	/* Bind our VAO as the current used object */
	glBindVertexArray(lightCubeVAO);
	/* Allocate and assign one VBO to our handle */
	glGenBuffers(1, &VBO);
	/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/* Copy the vertex data to our buffer */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Light position attribute */
	/* Specify that our coordinate data is going into attribute index 0, and contains 3 floats per vertex */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(0);

	/* NORMAL ATTRIBUTE */
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glBindVertexArray(lightCubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

bool Entity::intersectionBoxBounds(vec3 box_min, vec3 box_max)
{
	return overlap(box_min.x, box_max.x, min.x, max.x) && overlap(box_min.y, box_max.y, min.y, max.y) && overlap(box_min.z, box_max.z, min.z, max.z);
}