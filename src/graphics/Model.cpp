#include "graphics/Model.h"

#include "tracy/TracyOpenGL.hpp"

namespace graphics {

	Model::Model(Material material) : m_material(material) {
		ZoneScoped;
		TracyGpuZone("model upload");
		m_vao.bind();

		m_vertices.bind();
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0); // Vertex position

		m_normals.bind();
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1); // normals

		m_uvs.bind();
		glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2); // UVs

		glBindVertexArray(0);
	}
} // namespace graphics
