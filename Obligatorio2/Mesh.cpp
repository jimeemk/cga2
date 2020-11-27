#include "Mesh.h"


Mesh::Mesh(vector<Vertex> vertic, vector<GLuint> ind, vector<Texture> textur, vector<VertexBoneData> bone_id_weights)
{
	vertices = vertic;
	indices = ind;
	textures = textur;
	bones_id_weights_for_each_vertex = bone_id_weights;

	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
}

Mesh::~Mesh()
{
	//cout << "									 Mesh::~Mesh() " << endl;
	//glDeleteBuffers(1, &VBO_vertices);
	//glDeleteBuffers(1, &VBO_bones);
	//glDeleteBuffers(1, &EBO_indices);
	//glDeleteVertexArrays(1, &VAO);
}

void VertexBoneData::addBoneData(uint bone_id, float weight)
{
	for (uint i = 0; i < NUM_BONES_PER_VEREX; i++)
	{
		if (weights[i] == 0.0)
		{
			ids[i] = bone_id;
			weights[i] = weight;
			return;
		}
	}
}


void Mesh::Draw(GLuint shaders_program)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shaders_program, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	//vertices data
	glGenBuffers(1, &VBO_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bones data
	glGenBuffers(1, &VBO_bones);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
	glBufferData(GL_ARRAY_BUFFER, bones_id_weights_for_each_vertex.size() * sizeof(bones_id_weights_for_each_vertex[0]), &bones_id_weights_for_each_vertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//numbers for sequence indices
	glGenBuffers(1, &EBO_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// create VAO and binding data from buffers to shaders
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1); // offsetof(Vertex, normal) = returns the byte offset of that variable from the start of the struct
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//bones
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)0); // for INT Ipointer
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid*)offsetof(VertexBoneData, weights));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_indices);
	glBindVertexArray(0);
}