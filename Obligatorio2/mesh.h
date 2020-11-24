#pragma once
#include <iostream>

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "glm\glm.hpp"
#include "assimp\Importer.hpp"

#include <vector>
#include <string>

typedef unsigned int uint;
#define NUM_BONES_PER_VEREX 4

using namespace std;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};
struct BoneMatrix
{
    aiMatrix4x4 offset_matrix;
    aiMatrix4x4 final_world_transform;

};

struct VertexBoneData
{
    uint ids[NUM_BONES_PER_VEREX];   // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
    float weights[NUM_BONES_PER_VEREX];

    VertexBoneData()
    {
        memset(ids, 0, sizeof(ids));    // init all values in array = 0
        memset(weights, 0, sizeof(weights));
    }

    void addBoneData(uint bone_id, float weight);
};

class Mesh {
public:
    Mesh(vector<Vertex> vertic, vector<GLuint> ind, vector<Texture> textur, vector<VertexBoneData> bone_id_weights);
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    Mesh() {};
    ~Mesh();

    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    // Render mesh
    void Draw(GLuint shaders_program);


private:
    // render data 
    vector<VertexBoneData> bones_id_weights_for_each_vertex;

    //buffers
    GLuint VAO;
    GLuint VBO_vertices;
    GLuint VBO_bones;
    GLuint EBO_indices;

    //inititalize buffers
    void setupMesh();

};
