#pragma once
#include <GL/glew.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>

#include "gl_wrapper/Utils.h"

namespace gl_wrapper {

	using TextureDestructor = decltype(GL_DEST(id) { return glDeleteTextures(1, &id); });
	using TextureConstructor = decltype([]() -> GLuint {
		GLuint texture;
		glGenTextures(1, &texture);
		return texture;
	});

	enum class TextureBindTarget : GLenum { Texture2D = GL_TEXTURE_2D };

	enum class ImageFormat2D : GLenum { RGB = GL_RGB, BGR = GL_BGR, RGBA = GL_RGBA };

	class Texture: public GLObject<Texture, TextureConstructor, TextureDestructor> {
		protected:
			void bind_texture(TextureBindTarget target) {
				glBindTexture(std::to_underlying(target), m_raw_id);
			}
		public:
			static void set_active_texture(GLenum texture) {
				glActiveTexture(texture);
			}
	};

	class Texture2D : public Texture {
		public:
			void bind_texture() {
				Texture::bind_texture(TextureBindTarget::Texture2D);
			}	

			void upload_image(
				size_t width,
				size_t height,
				ImageFormat2D src_format,
				ImageFormat2D internal_format,
				std::span<uint8_t> data
			) {
				bind_texture();
				glTexImage2D(
					std::to_underlying(TextureBindTarget::Texture2D),
					0,
					std::to_underlying(internal_format),
					width,
					height,
					0,
					std::to_underlying(src_format),
					GL_UNSIGNED_BYTE,
					data.data()
				);
				// FIXME: shouldn't really be defaulted here
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
	};
} // namespace gl_wrapper