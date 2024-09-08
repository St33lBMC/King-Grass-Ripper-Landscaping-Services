#include "objmodel.h"

#include <GL/glew.h>
#include "objloader.h"
using namespace gl_wrapper;


ObjectModel::ObjectModel(const char path[], bool normalized) {
	bool res = loadOBJ(path, 
					   this -> vertices, 
					   this -> uvs, 
					   this -> normals);
	std::cout << "balls and cock " << this -> vertices.size() << std::endl;
	for (auto vertex : this -> vertices) {
		std::cout << glm::to_string(vertex) << std::endl;
	}
	glGenVertexArrays(1, &(this -> vao));
	glBindVertexArray(this -> vao);

	buffer::VertexBuffer<glm::vec3> vertices_buf;
	buffer::VertexBuffer<glm::vec3> normals_buf;
	buffer::VertexBuffer<glm::vec2> uvs_buf;

	vertices_buf.bind();
	vertices_buf.upload_data(this -> vertices, buffer::UsageHint::Static);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0); // Vertex position

	normals_buf.bind();
	normals_buf.upload_data(this -> normals, buffer::UsageHint::Static);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1); //normals

	uvs_buf.bind();
	uvs_buf.upload_data(this -> uvs, buffer::UsageHint::Static);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2); //UVs
	glBindVertexArray(0);
}

ObjectModel::~ObjectModel(){
	//todo: maybe?
}

void ObjectModel::drawModel() {
	glBindVertexArray(this -> vao);
	glDrawArrays(GL_TRIANGLES, 0, this -> vertices.size());
}

/*void createModel() {
	bool res = loadOBJ("../src/book.obj", vertices, uvs, normals);
	std::cout << "balls and cock " << vertices.size() << std::endl;
	for (auto vertex : vertices) {
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
	glEnableVertexAttribArray(0); // Vertex position

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
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
*/