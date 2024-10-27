#include "utils/Window.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <utility>

namespace utils {

	Window::~Window() {
		if (m_raw_window != nullptr)
			glfwDestroyWindow(m_raw_window);
	}

	Window::Window(Window&& w) {
		this->m_raw_window = w.m_raw_window;
		w.m_raw_window = nullptr;
	}

	void Window::swap_buffers() {
		glfwSwapBuffers(m_raw_window);
	}

	ButtonState Window::key_state(int key) {
		return static_cast<ButtonState>(glfwGetKey(m_raw_window, key));
	}

	ButtonState Window::mouse_button_state(int button) {
		return static_cast<ButtonState>(glfwGetMouseButton(m_raw_window, button));
	}

	bool Window::should_close() {
		return glfwWindowShouldClose(m_raw_window);
	}

	glm::dvec2 Window::cursor_position() {
		glm::dvec2 position;

		double* ptr = glm::value_ptr(position);
		glfwGetCursorPos(m_raw_window, &ptr[0], &ptr[1]);
		return position;
	}

	void Window::cursor_position(glm::dvec2& position) {
		glfwSetCursorPos(m_raw_window, position.x, position.y);
	}

	void Window::set_cursor_mode(CursorMode mode) {
		glfwSetInputMode(m_raw_window, GLFW_CURSOR, std::to_underlying(mode));
	}
} // namespace utils