#include <GL/glew.h>

namespace gl_wrapper {
	class VAO {
			GLuint m_raw_id;
			bool m_moved_from = false;

		public:
			VAO() {
				glGenVertexArrays(1, &(this->m_raw_id));
			}

			~VAO() {
				if (!m_moved_from)
					glDeleteVertexArrays(1, &this->m_raw_id);
			}

			VAO(VAO&& other) {
				this->m_raw_id = other.m_raw_id;
				other.m_moved_from = true;
			}

			void bind() {
				glBindVertexArray(m_raw_id);
			}
	};
} // namespace gl_wrapper