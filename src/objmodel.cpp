#include "objmodel.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

#include "objloader.h"

#include "gl_wrapper/buffer/Buffer.h"
using namespace gl_wrapper;

GLuint vao;
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;

void createModel()
{
	
    bool res = loadOBJ("../src/cube.obj", vertices, uvs, normals);
	std::cout << "balls and cock " << vertices.size() << std::endl;
	for(auto vertex : vertices) {
		std::cout << glm::to_string(vertex) << std::endl;
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	buffer::VertexBuffer<glm::vec3> vertices_buf;
	buffer::VertexBuffer<glm::vec3> normals_buf;
	buffer::VertexBuffer<glm::vec2> uvs_buf;
	
	vertices_buf.bind();
	vertices_buf.upload_data(vertices, buffer::UsageHint::Static);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	normals_buf.bind();
	normals_buf.upload_data(normals, buffer::UsageHint::Static);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1); //normals

	uvs_buf.bind();
	uvs_buf.upload_data(uvs, buffer::UsageHint::Static);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2); //UVs
	glBindVertexArray(0);
}

void drawModel() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}
