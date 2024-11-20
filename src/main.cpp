// Include standard headers
#include <fmt/core.h>
#include <fmt/format.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

//borrowed shader/obj parsing
#include "Game.h"
#include "gl_wrapper/texture/Texture.h"
#include "graphics/Model.h"
#include "graphics/image/Image.h"
#include "models/ObjectModel.h"
#include "objloader.h"
#include "tracy/TracyOpenGL.hpp"
#include "utils/Verify.h"

// Include glew, GLFW, glm
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//image file parser for textures
#include <png.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <sstream>
#include <vector>

#include "gl_wrapper/shader/Shader.h"
#include "gl_wrapper/shader/UniformImpl.h"
#include "graphics/text/Freetype.h"
#include "utils/Camera.h"
#include "utils/json/Parser.h"

using namespace gl_wrapper;
using namespace utils;

//libpng testing
// void read_texture(char* fname) {
// 	FILE* test = fopen(fname, "r");
// 	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
// 	png_init_io(png, test);
// 	auto info = png_create_info_struct(png);
// 	if (!info)
// 		std::cout << "unable to generate png info" << std::endl;
// 	png_set_sig_bytes(png, 0);
// 	png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);
// }

extern "C" void print_glerror(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam
) {
	fmt::print("OpenGL error: {}", message);
}

GLFWwindow* initialize() {
	// Initialize GLFW
	if (!glfwInit()) {
		PANIC("failed to initialise GLFW");
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
				   GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
				   GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(1024, 768, "balls 01", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		PANIC("failed to open GLFW window");
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//set up keyboardin n shiet

	glClearColor(0.82f, 0.875f, 0.933f, 0.0f);

	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	// glDebugMessageCallback(print_glerror, "abc");
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	TracyGpuContext;

	return window;
};

int main(void) {
	GLFWwindow* window = initialize();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	using namespace graphics::text;

	freetype::Library library;

	freetype::FontFace face(library, "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf", 0);

	face.set_char_size(16 * 64, 16 * 64, 300, 300);

	face.load_glyph(face.char_index('O'), 0);
	face.render_glyph();

	auto tex = std::make_shared<Texture2D>();
	tex->bind_texture();
	glTexImage2D(
		std::to_underlying(TextureBindTarget::Texture2D),
		0,
		std::to_underlying(ImageFormat2D::Red),
		face.raw()->glyph->bitmap.width,
		face.raw()->glyph->bitmap.rows,
		0,
		std::to_underlying(ImageFormat2D::Red),
		GL_UNSIGNED_BYTE,
		face.raw()->glyph->bitmap.buffer
	);
	// FIXME: shouldn't really be defaulted here
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	fmt::print("Char index of a: {}\n", face.raw()->glyph->bitmap.pixel_mode);

	// auto img = graphics::RGBA8888Image::from_file("/home/exo/Documents/thebuild.png");

	// auto tex = std::make_shared<Texture2D>();
	// tex->upload_image(img);

	Game game((Window(window)));
	game.camera().m_aspect = 1024.f / 768.f;
	models::ObjectModel cube_obj("../src/cube.obj");
	graphics::Model cube(graphics::Material(glm::vec4(0, 1, 0, 1), tex));
	cube_obj.upload_to(cube);
	cube.transform() = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0));

	models::ObjectModel book_obj("../src/book.obj");
	graphics::Model book(graphics::Material(glm::vec4(1, 0, 0, 1), tex));
	book_obj.upload_to(book);
	book.transform() = glm::translate(glm::mat4(1.0f), glm::vec3(0, 4, 0));

	std::vector<graphics::Model> models;
	models.push_back(std::move(cube));
	models.push_back(std::move(book));

	game.loop(std::move(models));
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
