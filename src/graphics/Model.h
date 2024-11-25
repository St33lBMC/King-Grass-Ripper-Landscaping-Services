#pragma once

#include <glm/ext.hpp>
#include <iostream>

#include "gl_wrapper/VAO.h"
#include "gl_wrapper/buffer/Buffer.h"
#include "gl_wrapper/shader/Shader.h"
#include "graphics/Material.h"
#include "tracy/TracyOpenGL.hpp"

using namespace gl_wrapper::buffer;
using namespace gl_wrapper;

namespace graphics {
	class Model {
			VertexBuffer<glm::vec3> m_vertices;
			VertexBuffer<glm::vec3> m_normals;
			VertexBuffer<glm::vec2> m_uvs;
			VAO m_vao;
			glm::mat4 m_model_transform = glm::mat4(1.0f);
			Material m_material;

		public:
			auto& vertices() {
				return m_vertices;
			}

			auto& normals() {
				return m_normals;
			}

			auto& uvs() {
				return m_uvs;
			}

			Material& material() {
				return m_material;
			}

			glm::mat4& transform() {
				return m_model_transform;
			}

			Model(Material material);

			void draw(shader::Program& program) {
				ZoneScoped;
				TracyGpuZone("model draw");
				program.set_uniform("model_matrix", transform());
				m_material.upload_uniform(program, "material");
				m_vao.bind();
				glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
			}
	};
} // namespace graphics
