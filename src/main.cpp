// Include standard headers
#include <fmt/core.h>
#include <fmt/format.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/random.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <string>
#include <unordered_set>

//borrowed shader/obj parsing
#include "Game.h"
#include "ecs/archetypal/Archetype.h"
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

#include "ecs/Query.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include "ecs/archetypal/Archetype.h"
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
	(void)source;
	(void)type;
	(void)id;
	(void)severity;
	(void)length;
	(void)userParam;
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

	// ecs::Query<ecs::Component<int&>, ecs::Component<int const&>> query;

	// fmt::print("Uniqueness of {}, {}\n", query.m_unique_set[0], query.m_unique_set[1]);

	// ecs::TypeKey key1 = ecs::TypeKey::create<int, long, ShaderIV>();
	// ecs::TypeKey key2 = ecs::TypeKey::create<long, ShaderIV, int>();
	// fmt::print("equal? {}\n", key1 == key2);

	struct Sheezer {
			float g;
	};

	ecs::archetypal::Archetype a = ecs::archetypal::Archetype::create<int, long>();
	a.add(4, 5l);
	a.add(6l, 7);
	a.add(9l, 21);

	using namespace graphics;

	text::Library library;

	text::FontFace face(library, "/usr/share/fonts/truetype/freefont/FreeSerif.ttf", 0);

	face.set_pixel_size(0, 512);
	auto load_flags = FT_FACE_FLAG_KERNING;
	face.load_glyph(face.char_index('g'), load_flags);
	face.render_glyph();

	auto& glyph = face.raw()->glyph;

	fmt::print("Left {} top {}\n", glyph->bitmap_left, glyph->bitmap_top);
	auto tex = std::make_shared<Texture2D>();
	tex->upload_image(face.bitmap());

	fmt::print("Char index of a: {}\n", face.raw()->glyph->bitmap.pixel_mode);

	// auto img = graphics::from_file("/home/exo/Documents/thebuild.png");

	// auto tex = std::make_shared<Texture2D>();
	// tex->upload_image(graphics::ImageRef<graphics::PixelFormat::RGBA8888> { img });

	models::ObjectModel cube_obj("../assets/models/cube.obj");
	graphics::Model cube(graphics::Material(glm::vec4(0, 1, 0, 1), tex));
	cube_obj.upload_to(cube);
	cube.transform() = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0));

	models::ObjectModel book_obj("../assets/models/book.obj");
	graphics::Model book(graphics::Material(glm::vec4(1, 0, 0, 1), tex));
	book_obj.upload_to(book);

	// std::vector<graphics::Model> models;
	// models.push_back(std::move(cube));
	// models.push_back(std::move(book));

	ecs::World world;
	world.add_system<ecs::Query<ecs::Component<glm::vec3&>>>(
		[](ecs::CommandQueue&, ecs::WorldView world, ecs::Query<ecs::Component<glm::vec3&>> query) {
			world.satisfy(query, [](glm::vec3& position) {
				//fmt::print("this does indeed have side effects\n");
				position += glm::sphericalRand(0.1);
			});
		}
	);

	world.add<glm::vec3, graphics::Model>(glm::vec3(0, 0, 1), std::forward<graphics::Model&&>(cube));
	world.add<glm::vec3, graphics::Model>(glm::vec3(1, 0, 0), std::forward<graphics::Model&&>(book));

	Game game((Window(window)), std::move(world));
	game.camera().m_aspect = 1024.f / 768.f;

	game.loop();
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
