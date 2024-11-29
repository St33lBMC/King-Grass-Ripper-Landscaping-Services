#include <GL/glew.h>

#include "gl_wrapper/Utils.h"

namespace gl_wrapper {
	using VAODestructor = decltype(GL_DEST(id) { return glDeleteVertexArrays(1, &id); });
	using VAOConstructor = decltype([]() -> GLuint {
		GLuint vao;
		glGenVertexArrays(1, &vao);
		return vao;
	});

	class VAO: public GLObject<VAO, VAOConstructor, VAODestructor> {
		public:
			void bind() const {
				glBindVertexArray(m_raw_id);
			}
	};
} // namespace gl_wrapper
