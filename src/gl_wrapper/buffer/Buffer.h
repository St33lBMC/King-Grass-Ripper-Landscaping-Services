#include <GL/glew.h>

#include <cstddef>
#include <iostream>
#include <span>
#include <utility>

namespace gl_wrapper::buffer {

	enum class BufferBindingTarget : GLenum { ArrayBuffer = GL_ARRAY_BUFFER, IndexBuffer = GL_ELEMENT_ARRAY_BUFFER };

	enum class UsageHint : GLenum { Static = GL_STATIC_DRAW };

	enum class BufferIV : GLenum { BufferSize = GL_BUFFER_SIZE, BufferUsage = GL_BUFFER_USAGE };

	class Buffer {
			GLuint m_raw_id;
			bool m_moved_from = false;

		public:
			Buffer() {
				glGenBuffers(1, &m_raw_id);
			}

			~Buffer() {
				if (!m_moved_from)
					glDeleteBuffers(1, &m_raw_id);
			}

			Buffer(Buffer&& other) {
				this->m_raw_id = other.m_raw_id;
				other.m_moved_from = true;
			}

			GLuint raw_id() {
				return m_raw_id;
			}

			void bind(BufferBindingTarget target) {
				glBindBuffer((GLenum)target, m_raw_id);
			}

			template<typename T>
			void upload_data(std::span<T> data, BufferBindingTarget bind_target, UsageHint usage_hint) {
				bind(bind_target);
				glBufferData((GLenum)bind_target, sizeof(T) * data.size(), data.data(), (GLenum)usage_hint);
			}

			GLint get_info(BufferBindingTarget bind_target, BufferIV iv) {
				this->bind(bind_target);
				GLint output;
				glGetBufferParameteriv((GLenum)bind_target, std::to_underlying(iv), &output);
				return output;
			}
	};

	template<typename T> class VertexBuffer: Buffer {
			size_t m_num_elements;

		public:
			VertexBuffer() : Buffer() {
				Buffer::bind(BufferBindingTarget::ArrayBuffer);
			}

			void bind() {
				Buffer::bind(BufferBindingTarget::ArrayBuffer);
			}

			size_t size() {
				return m_num_elements;
			}

			void upload_data(std::span<T> data, UsageHint usage_hint) {
				m_num_elements = data.size();
				Buffer::upload_data(data, BufferBindingTarget::ArrayBuffer, usage_hint);
			}

			GLint get_info(BufferIV iv) {
				return Buffer::get_info(BufferBindingTarget::ArrayBuffer, iv);
			}
	};

}; // namespace gl_wrapper::buffer