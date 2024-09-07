// Include standard headers
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstdlib>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

//borrowed shader/obj parsing
#include "assets/manager/AssetManager.h"
#include "assets/provider/FileAssetProvider.h"
#include "assets/types/TextAsset.h"
#include "objloader.h"

//arbitrary model class
#include "objmodel.h"

// Include glew, GLFW, glm
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "gl_wrapper/shader/Shader.h"
#include "gl_wrapper/shader/UniformImpl.h"
#include "utils/Camera.h"

using namespace gl_wrapper;
using namespace utils;

//constant camera speed (move to camera class?)
const float camera_speed = 0.1f;

//evil globals >:)
// VVV this one is more evil than most
shader::Program* program = nullptr;

GLuint compile_shaders() {
	return 0;
}

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

void handle_keypress(GLFWwindow* window, Camera& camera, double& cursor_x, double& cursor_y) {
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.m_position -= camera_speed * camera.forward();
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.m_position += camera_speed * camera.forward();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.m_position += camera_speed * camera.right();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.m_position -= camera_speed * camera.right();
	}
	double new_cursor_x = 0.0;
	double new_cursor_y = 0.0;
	glfwGetCursorPos(window, &new_cursor_x, &new_cursor_y);

	double x_delta = new_cursor_x - cursor_x;
	double y_delta = new_cursor_y - cursor_y;

	cursor_x = new_cursor_x;
	cursor_y = new_cursor_y;

	camera.m_pitch += y_delta * 0.1;
	camera.m_yaw += x_delta * 0.1;

	camera.m_pitch = glm::min(camera.m_pitch, 90.0f);
	camera.m_pitch = glm::max(camera.m_pitch, -90.0f);

	//std::cout << camera.m_pitch << " balls, even " << camera.m_yaw << std::endl;
}

GLFWwindow* initialize() {
	std::unique_ptr<assets::provider::FileAssetProvider> asset_provider =
		std::make_unique<assets::provider::FileAssetProvider>("../src/");

	assets::manager::AssetManager manager = assets::manager::AssetManager(std::move(asset_provider));

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

	// Dark blue background
	glClearColor(0.4f, 0.0f, 0.4f, 0.0f);

	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(print_glerror, nullptr);

	try {
		program = new shader::Program();

		auto vertex_shader = shader::Shader(shader::ShaderType::Vertex);
		auto fragment_shader = shader::Shader(shader::ShaderType::Fragment);

		auto vs = manager.get_asset<assets::types::TextAsset>("shaders/shader.vs");

		vertex_shader.upload_shader_source(vs->text());

		vertex_shader.compile_shader();

		auto fs = manager.get_asset<assets::types::TextAsset>("shaders/shader.fs");

		fragment_shader.upload_shader_source(fs->text());

		fragment_shader.compile_shader();

		program->link([&](shader::Linking& l) {
			l.attach_shader(vertex_shader);
			l.attach_shader(fragment_shader);

			l.bind_attribute_location("position", 0);
		});
		//programID =
		program->use_program();
	} catch (shader::ShaderException& e) {
		std::cerr << "SHADER ERRR: " << e.what() << std::endl;
		exit(-1);
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	return window;
};

int main(void) {
	GLFWwindow* window = initialize();
	double cursor_x = 0.0;
	double cursor_y = 0.0;
	Camera camera(glm::vec3(0, 0, 0));
	glm::mat4 model_matrix = glm::mat4(1.0);
	model_matrix = glm::translate(model_matrix, glm::vec3(0, 0, 0));

	program->set_uniform("model_matrix", model_matrix);
	program->set_uniform("projection_matrix", camera.perspective());
	program->set_uniform("view_matrix", camera.view());
	createModel();
	do {
		//handle keypresses to update camera position/direction
		handle_keypress(window, camera, cursor_x, cursor_y);

		program->set_uniform("view_matrix", camera.view());

		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw model(s)
		drawModel();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		const auto err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cout << err << std::endl;
		}

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
