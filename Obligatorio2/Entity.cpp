#include "Entity.h"

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
    cout << "minAfterRot: " << min.x << " ... " << min.y << " ... " << min.z << "\n";
    cout << "maxAfterRot: " << max.x << " ... " << max.y << " ... " << max.z << "\n";
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

