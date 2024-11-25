#pragma once
#include <GL/glew.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>

#include "gl_wrapper/Utils.h"
#include "graphics/image/Image.h"
#include "utils/Verify.h"

namespace gl_wrapper {

	using TextureDestructor = decltype(GL_DEST(id) { return glDeleteTextures(1, &id); });
	using TextureConstructor = decltype([]() -> GLuint {
		GLuint texture;
		glGenTextures(1, &texture);
		return texture;
	});

	enum class TextureBindTarget : GLenum { Texture2D = GL_TEXTURE_2D };

	enum class ImageFormat2D : GLenum { RGB = GL_RGB, BGR = GL_BGR, RGBA = GL_RGBA, Red = GL_RED };

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

	class Texture2D: public Texture {
		public:
			void bind_texture() {
				Texture::bind_texture(TextureBindTarget::Texture2D);
			}

			template<graphics::PixelFormat T> void upload_image(graphics::ImageRef<T> image) {
				bind_texture();
				ImageFormat2D gl_image_format;

				switch (T) {
					case graphics::PixelFormat::RGBA8888:
						gl_image_format = ImageFormat2D::RGBA;
						break;
					case graphics::PixelFormat::RGB888:
						gl_image_format = ImageFormat2D::RGB;
						break;
					case graphics::PixelFormat::Grayscale8:
						gl_image_format = ImageFormat2D::Red;
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
						break;
					default:
						PANIC("unreachable")
				};

				glTexImage2D(
					std::to_underlying(TextureBindTarget::Texture2D),
					0,
					std::to_underlying(gl_image_format),
					image.width(),
					image.height(),
					0,
					std::to_underlying(gl_image_format),
					GL_UNSIGNED_BYTE,
					image.data().data()
				);
				// FIXME: shouldn't really be defaulted here
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
	};
} // namespace gl_wrapper
