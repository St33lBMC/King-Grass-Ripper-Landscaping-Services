#pragma once
#include <GL/glew.h>

#include <glm/ext.hpp>

namespace gl_wrapper::shader {
	void upload_uniform(glm::mat4 mat, GLint target) {
		glUniformMatrix4fv(
			target,
			1,
			GL_FALSE,
			&mat[0][0]
		);
	}
} // namespace gl_wrapper::shader