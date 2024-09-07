#include "objmodel.h"
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "objloader.h"
#include <iostream>

GLuint vao;
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;

void createModel()
{
	
    bool res = loadOBJ("../src/cube.obj", vertices, uvs, normals);
	std::cout << "balls and cock " << vertices.size() << std::endl;
	for(int i = 0; i < vertices.size(); i++) {
		std::cout << vertices.at(i).x << " " << vertices.at(i).y << " " << vertices.at(i).z << std::endl;
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1); //normals

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2); //UVs
	glBindVertexArray(0);

}

void drawModel() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}


