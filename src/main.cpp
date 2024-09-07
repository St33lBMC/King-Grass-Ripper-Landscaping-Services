// Include standard headers
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

//borrowed shader/obj parsing
#include "objloader.h"
#include "shader.h"

//arbitrary model class
#include "objmodel.h"

// Include glew, GLFW, glm
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include "gl_wrapper/shader/Shader.h"

//evil globals >:)
GLFWwindow* window;
GLuint programID;

float camera_speed = 0.1f;
float horizontal_angle = 0.0f;
float vertical_angle = 0.0f;
float pitch = 0.0f;
float yaw = 0.0f;

double cursor_x = 0.0;
double cursor_y = 0.0;

glm::vec3 camera_pos = glm::vec3(0, 0, 0);
glm::vec3 direction_vector = glm::vec3(0, 0, 0);
glm::vec3 target_vector = glm::vec3(0, 0, 0);

GLuint compile_shaders() {
	return 0;
}

void balls(
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

glm::vec3 compute_right() {
	return glm::vec3(sin(glm::radians(yaw) - glm::pi<float>() / 2.0), 0.0, cos(glm::radians(yaw) - glm::pi<float>() / 2.0));
};

void dokeyboardshitmonica() {
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_pos -= camera_speed * direction_vector;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_pos += camera_speed * direction_vector;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_pos += camera_speed * compute_right();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_pos -= camera_speed * compute_right(); 
	}
	double new_cursor_x = 0.0;
	double new_cursor_y = 0.0;
	glfwGetCursorPos(window, &new_cursor_x, &new_cursor_y);

	double x_delta = new_cursor_x - cursor_x;
	double y_delta = new_cursor_y - cursor_y;

	cursor_x = new_cursor_x;
	cursor_y = new_cursor_y;

	// pitch = 0.0;
	// yaw = 0.0;

	pitch += y_delta * 0.1;
	yaw += x_delta * 0.1;

	pitch = glm::min(pitch, 180.0f);
	pitch = glm::max(pitch, -180.0f);


	
	direction_vector.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	direction_vector.y = sin(glm::radians(pitch));
	direction_vector.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction_vector = glm::normalize(direction_vector);

	
	std::cout << new_cursor_x << " balls, even " << new_cursor_y << std::endl;
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
	glfwWindowHint(
		GLFW_OPENGL_FORWARD_COMPAT,
		GL_TRUE
	); // To make macOS happy; should not be needed
	glfwWindowHint(
		GLFW_OPENGL_FORWARD_COMPAT,
		GL_TRUE
	); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "balls 01", NULL, NULL);
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
	glDebugMessageCallback(balls, nullptr);
	programID =
		LoadShaders("../src/shaders/shader.vs", "../src/shaders/shader.fs");
	glUseProgram(programID);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	return window;
};

int main(void) {
	GLFWwindow* window = initialize();
	glm::mat4 model_matrix = glm::mat4(1.0);
	glm::mat4 projection_matrix =
		glm::perspective(glm::radians(35.0f), 1.0f, 0.1f, 100.0f);
	glm::mat4 view_matrix =
		glm::lookAt(camera_pos, glm::vec3(3, -2, -4), glm::vec3(0, 1, 0));
	model_matrix = glm::translate(model_matrix, glm::vec3(3, -2, -4));
	glUniformMatrix4fv(
		glGetUniformLocation(programID, "model_matrix"),
		1,
		GL_FALSE,
		&model_matrix[0][0]
	);
	glUniformMatrix4fv(
		glGetUniformLocation(programID, "projection_matrix"),
		1,
		GL_FALSE,
		&projection_matrix[0][0]
	);
	glUniformMatrix4fv(
		glGetUniformLocation(programID, "view_matrix"),
		1,
		GL_FALSE,
		&view_matrix[0][0]
	);
	createModel();
	do {
		//do keybord shit monica
		dokeyboardshitmonica();

		std::cout << "Camera position: " << glm::to_string(camera_pos) << std::endl;
		std::cout << "Dir. vector: " << glm::to_string(direction_vector) << std::endl;

		view_matrix =
			glm::lookAt(camera_pos, camera_pos + direction_vector, glm::cross(direction_vector, compute_right()));
		glUniformMatrix4fv(
			glGetUniformLocation(programID, "view_matrix"),
			1,
			GL_FALSE,
			&view_matrix[0][0]
		);
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT);
		// Draw nothing, see you in tutorial 2 !
		drawModel();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		const auto err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cout << err << std::endl;
		}

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		   && glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
