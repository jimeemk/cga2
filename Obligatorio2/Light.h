#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
using namespace glm;
using namespace std;

class Light {
public:
	vec3 position;
	vec3 color;

	Light(vec3 position) {
		this->position = position;
		this->color = vec3(1, 1, 1);
	}

	Light(vec3 position, vec3 color) {
		this->position = position;
		this->color = color;
	}

	void drawLight() {
		float vertices[] = {
		-5 + position.x, -5 + position.y, -0.5f + position.z,  0.0f,  0.0f, -1.0f,
		 5 + position.x, -5 + position.y, -0.5f + position.z,  0.0f,  0.0f, -1.0f,
		 5 + position.x,  5 + position.y, -0.5f + position.z,  0.0f,  0.0f, -1.0f,
		 5 + position.x,  5 + position.y, -0.5f + position.z,  0.0f,  0.0f, -1.0f,
		-5 + position.x,  5 + position.y, -0.5f + position.z,  0.0f,  0.0f, -1.0f,
		-5 + position.x, -5 + position.y, -0.5f + position.z,  0.0f,  0.0f, -1.0f,

		-5 + position.x, -5 + position.y,  0.5f + position.z,  0.0f,  0.0f,  1.0f,
		 5 + position.x, -5 + position.y,  0.5f + position.z,  0.0f,  0.0f,  1.0f,
		 5 + position.x,  5 + position.y,  0.5f + position.z,  0.0f,  0.0f,  1.0f,
		 5 + position.x,  5 + position.y,  0.5f + position.z,  0.0f,  0.0f,  1.0f,
		-5 + position.x,  5 + position.y,  0.5f + position.z,  0.0f,  0.0f,  1.0f,
		-5 + position.x, -5 + position.y,  0.5f + position.z,  0.0f,  0.0f,  1.0f,

		-5 + position.x,  5 + position.y,  0.5f + position.z, -1.0f,  0.0f,  0.0f,
		-5 + position.x,  5 + position.y, -0.5f + position.z, -1.0f,  0.0f,  0.0f,
		-5 + position.x, -5 + position.y, -0.5f + position.z, -1.0f,  0.0f,  0.0f,
		-5 + position.x, -5 + position.y, -0.5f + position.z, -1.0f,  0.0f,  0.0f,
		-5 + position.x, -5 + position.y,  0.5f + position.z, -1.0f,  0.0f,  0.0f,
		-5 + position.x,  5 + position.y,  0.5f + position.z, -1.0f,  0.0f,  0.0f,

		 5 + position.x,  5 + position.y,  0.5f + position.z,  1.0f,  0.0f,  0.0f,
		 5 + position.x,  5 + position.y, -0.5f + position.z,  1.0f,  0.0f,  0.0f,
		 5 + position.x, -5 + position.y, -0.5f + position.z,  1.0f,  0.0f,  0.0f,
		 5 + position.x, -5 + position.y, -0.5f + position.z,  1.0f,  0.0f,  0.0f,
		 5 + position.x, -5 + position.y,  0.5f + position.z,  1.0f,  0.0f,  0.0f,
		 5 + position.x,  5 + position.y,  0.5f + position.z,  1.0f,  0.0f,  0.0f,

		-5 + position.x, -5 + position.y, -0.5f + position.z,  0.0f, -1.0f,  0.0f,
		 5 + position.x, -5 + position.y, -0.5f + position.z,  0.0f, -1.0f,  0.0f,
		 5 + position.x, -5 + position.y,  0.5f + position.z,  0.0f, -1.0f,  0.0f,
		 5 + position.x, -5 + position.y,  0.5f + position.z,  0.0f, -1.0f,  0.0f,
		-5 + position.x, -5 + position.y,  0.5f + position.z,  0.0f, -1.0f,  0.0f,
		-5 + position.x, -5 + position.y, -0.5f + position.z,  0.0f, -1.0f,  0.0f,

		-5 + position.x,  5 + position.y, -0.5f + position.z,  0.0f,  1.0f,  0.0f,
		 5 + position.x,  5 + position.y, -0.5f + position.z,  0.0f,  1.0f,  0.0f,
		 5 + position.x,  5 + position.y,  0.5f + position.z,  0.0f,  1.0f,  0.0f,
		 5 + position.x,  5 + position.y,  0.5f + position.z,  0.0f,  1.0f,  0.0f,
		-5 + position.x,  5 + position.y,  0.5f + position.z,  0.0f,  1.0f,  0.0f,
		-5 + position.x,  5 + position.y, -0.5f + position.z,  0.0f,  1.0f,  0.0f
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
		/* Enable attribute index 0 as being used */
		glEnableVertexAttribArray(0);

		/* NORMAL ATTRIBUTE */
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
		//glEnableVertexAttribArray(1);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

};