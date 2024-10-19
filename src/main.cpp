// Include standard headers
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
#include <fmt/format.h>

//borrowed shader/obj parsing
#include "Game.h"
#include "gl_wrapper/texture/Texture.h"
#include "graphics/Model.h"
#include "graphics/image/Image.h"
#include "models/ObjectModel.h"
#include "objloader.h"
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
	std::cout << message << std::endl;
}

GLFWwindow* initialize() {
	// Initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		exit(-1);
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
		fprintf(
			stderr,
			"Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n"
		);
		getchar();
		glfwTerminate();
		exit(-1);
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
	std::cout << "AB2C" << std::endl;
	// glDebugMessageCallback(print_glerror, "abc");
	std::cout << "IN2NIT" << std::endl;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	return window;
};

int main(void) {
	//read_texture("");

	auto stream = std::istringstream("[true, 1, \"abc\"]");
	// json::Tokenized tokenized(stream);
	// std::cout << "RATHER: " << tokenized.peek().m_token_value.number << std::endl;

	// std::cout << "got: ";
	// json::JSONValue::parse(tokenized)->display(std::cout);
	// std::cout << std::endl;
	GLFWwindow* window = initialize();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto img = graphics::RGBA8888Image::from_file("/home/exo/Documents/oak_planks.png");

	auto tex = std::make_shared<Texture2D>();
	tex->upload_image(img);

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
