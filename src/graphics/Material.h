#include <GL/glew.h>

#include <glm/ext.hpp>
#include <memory>
#include <string>

#include "gl_wrapper/shader/Shader.h"
#include "gl_wrapper/texture/Texture.h"

using namespace gl_wrapper;

namespace graphics {
	struct Material {
			glm::vec4 m_colour;
			std::shared_ptr<Texture2D> m_texture;

			explicit Material(glm::vec4 colour, std::shared_ptr<Texture2D> texture) :
				m_colour(colour),
				m_texture(texture) {}

			void upload_uniform(gl_wrapper::shader::Program& program, std::string base) {
				program.set_uniform(base + ".colour", m_colour);
				// FIXME: texture units?
				Texture::set_active_texture(GL_TEXTURE0);
				program.set_uniform("textureSampler", 0);
				m_texture->bind_texture();
			}
	};

}; // namespace graphics

namespace gl_wrapper::uniforms {
	template<> inline void upload_uniform<graphics::Material>(graphics::Material& value, GLint v) {
		glUniform4fv(v, 1, &value.m_colour[0]);
	};
}; // namespace gl_wrapper::uniforms