#pragma once
#include <fmt/core.h>

#include <glm/ext.hpp>
#include <vector>

#include "gl_wrapper/VAO.h"
#include "gl_wrapper/buffer/Buffer.h"
#include "gl_wrapper/shader/Shader.h"
#include "utils/FileAccess.h"

namespace graphics {
	using namespace gl_wrapper;

	struct ParticleMarker {};

	class ParticleRenderer {
			shader::Program m_shader_program;

			VAO m_vao;
			buffer::VertexBuffer<glm::vec3> m_positions;
			buffer::VertexBuffer<glm::vec4> m_colours;

			struct Particle {
					glm::vec3 position;
					glm::vec4 colour;

					Particle(glm::vec3 position, glm::vec4 colour) : position(position), colour(colour) {}
			};

			std::vector<glm::vec3> m_local_positions;
			std::vector<glm::vec4> m_local_colours;

		public:
			shader::Program& shader() {
				return m_shader_program;
			}

			ParticleRenderer() {
				auto vertex_shader = shader::Shader(shader::ShaderType::Vertex);
				auto fragment_shader = shader::Shader(shader::ShaderType::Fragment);

				vertex_shader.upload_shader_source(utils::get_file_contents("../assets/shaders/particles/shader.vs"));

				vertex_shader.compile_shader();

				fragment_shader.upload_shader_source(utils::get_file_contents("../assets/shaders/particles/shader.fs"));

				fragment_shader.compile_shader();

				m_shader_program.link([&](shader::Linking& l) {
					l.attach_shader(vertex_shader);
					l.attach_shader(fragment_shader);

					l.bind_attribute_location("position", 0);
				});

				m_shader_program.use_program();
				m_shader_program.set_uniform("model_matrix", glm::mat4(1.0f));
				m_vao.bind();
				m_positions.bind();
				glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(0); // position

				m_colours.bind();
				glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(1); // colours
			}

			void enqueue_particle(Particle const& particle) {
				m_local_colours.push_back(particle.colour);
				m_local_positions.push_back(particle.position);
			}

			void draw_all() {
				m_shader_program.use_program();
				m_positions.upload_data(m_local_positions, buffer::UsageHint::Dynamic);
				m_colours.upload_data(m_local_colours, buffer::UsageHint::Dynamic);

				m_vao.bind();

				glDrawArrays(GL_POINTS, 0, m_local_positions.size());

				m_local_positions.clear();
				m_local_colours.clear();
			}
	};
} // namespace graphics
