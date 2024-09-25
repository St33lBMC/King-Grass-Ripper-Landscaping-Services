#pragma once
#include "gl_wrapper/Utils.h"
namespace gl_wrapper {

    using TextureDestructor = decltype(GL_DEST(id) { return glDeleteTextures(1, &id); });
	using TextureConstructor = decltype([]() -> GLuint {
		GLuint texture;
		glGenTextures(1, &texture);
		return texture;
	});
    class Texture : public GLObject<Texture, TextureConstructor, TextureDestructor> {

    };
}