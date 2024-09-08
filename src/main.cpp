// Include standard headers
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstdlib>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

//borrowed shader/obj parsing
#include "Game.h"
#include "assets/manager/AssetManager.h"
#include "assets/provider/FileAssetProvider.h"
#include "assets/types/TextAsset.h"
#include "graphics/Model.h"
#include "models/ObjectModel.h"
#include "objloader.h"

//arbitrary model class
// #include "objmodel.h"

// Include glew, GLFW, glm
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <vector>

#include "gl_wrapper/shader/Shader.h"
#include "gl_wrapper/shader/UniformImpl.h"
#include "utils/Camera.h"

using assets::provider::FileAssetProvider;

using namespace gl_wrapper;
using namespace utils;

void print_glerror(
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

	glDebugMessageCallback(print_glerror, nullptr);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	return window;
};

int main(void) {
	GLFWwindow* window = initialize();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	std::unique_ptr<FileAssetProvider> asset_provider = std::make_unique<FileAssetProvider>("../src/");

	Game game(Window(window), std::move(asset_provider));
	models::ObjectModel cube_obj("../src/cube.obj");
	graphics::Model cube(graphics::Material(glm::vec4(0, 1, 0, 1)));
	cube_obj.upload_to(cube);
	cube.transform() = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0));

	models::ObjectModel book_obj("../src/book.obj");
	graphics::Model book(graphics::Material(glm::vec4(1, 0, 0, 1)));
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
