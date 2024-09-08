#include <GL/glew.h>

#include <glm/ext.hpp>
#include <string>

#include "gl_wrapper/shader/Shader.h"

namespace graphics {
	struct Material {
			glm::vec4 m_colour;

			explicit Material(glm::vec4 colour) : m_colour(colour) {}

			void upload_uniform(gl_wrapper::shader::Program& program, std::string base) {
				program.set_uniform(base + ".colour", m_colour);
			}
	};

}; // namespace graphics

namespace gl_wrapper::uniforms {
	template<> inline void upload_uniform<graphics::Material>(graphics::Material& value, GLint v) {
		glUniform4fv(v, 1, &value.m_colour[0]);
	};
}; // namespace gl_wrapper::uniforms