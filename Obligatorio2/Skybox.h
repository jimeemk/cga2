#pragma once

#include <glm/glm.hpp>
#include <glad\glad.h>
#include "Shader.h"
#include <vector>

class Skybox {
public:
    Skybox() {
        float skyboxVertices[108] =
        {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);


        std::vector<std::string> faces =
        {
            "skybox/right.jpg",
            "skybox/left.jpg",
            "skybox/bottom.jpg",
            "skybox/top.jpg",
            "skybox/front.jpg",
            "skybox/back.jpg"
        };

        cubemapTexture = loadCubemap(faces);

        shader = new Shader("skybox.vert", "skybox.frag");

        shader->use();
        shader->setInt("skybox", 0);
    }

    ~Skybox() {}

    void Draw(const glm::mat4& view, const glm::mat4& projection) {
        glDepthFunc(GL_LEQUAL);
        shader->use();
        shader->setMat4("view", glm::mat4(glm::mat3(view)));
        shader->setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

private:
    unsigned int cubemapTexture;
    unsigned int VAO;
    unsigned int VBO;
    Shader* shader;
    
    unsigned int loadCubemap(std::vector<std::string> faces) {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++) {
            FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(faces[i].c_str(), 0), faces[i].c_str());
            FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
            int width = FreeImage_GetWidth(pImage);
            int height = FreeImage_GetHeight(pImage);

            if (pImage) {
                glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*) FreeImage_GetBits(pImage));
                FreeImage_Unload(pImage);
            } else {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
                FreeImage_Unload(pImage);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

};