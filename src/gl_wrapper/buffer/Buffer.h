#include <GL/glew.h>

#include <cstddef>
#include <span>

namespace gl_wrapper::buffer {

	enum class BufferBindingTarget : GLenum { ArrayBuffer = GL_ARRAY_BUFFER, IndexBuffer = GL_ELEMENT_ARRAY_BUFFER };

	enum class UsageHint : GLenum { Static = GL_STATIC_DRAW };

	class Buffer {
			GLuint m_raw_id;

		public:
			Buffer() {
				glGenBuffers(1, &m_raw_id);
			}

			~Buffer() {
				glDeleteBuffers(1, &m_raw_id);
			}

			GLuint raw_id() {
				return m_raw_id;
			}

			void bind(BufferBindingTarget target) {
				glBindBuffer((GLenum)target, m_raw_id);
			}

			template<typename T>
			void upload_data(std::span<T> data, BufferBindingTarget bind_target, UsageHint usage_hint) {
				glBufferData((GLenum)bind_target, sizeof(T) * data.size(), data.data(), (GLenum)usage_hint);
			}
	};

	template<typename T> class VertexBuffer: Buffer {
		public:
			VertexBuffer() : Buffer() {
				Buffer::bind(BufferBindingTarget::ArrayBuffer);
			}

			void bind() {
				Buffer::bind(BufferBindingTarget::ArrayBuffer);
			}

			void upload_data(std::span<T> data, UsageHint usage_hint) {
				Buffer::upload_data(data, BufferBindingTarget::ArrayBuffer, usage_hint);
			}
	};

}; // namespace gl_wrapper::buffer