#pragma once
#include <GL/glew.h>

namespace gl_wrapper {

	template<typename T, typename Constructor, typename Destructor, class... ConstructorArgs> class GLObject {
		private:
			bool m_moved_from = false;
		protected:
			GLuint m_raw_id;

			GLObject(ConstructorArgs... args) {
				m_raw_id = Constructor {}(args...);
			}

		public:
			virtual ~GLObject() {
				if (!m_moved_from)
					Destructor {}(m_raw_id);
			}

			GLObject(GLObject&& other) {
				other.m_moved_from = true;
				this->m_raw_id = other.m_raw_id;
			}

			GLuint raw_id() {
				return m_raw_id;
			}
	};

#define GL_DEST(id) [](GLuint id) -> void

} // namespace gl_wrapper
