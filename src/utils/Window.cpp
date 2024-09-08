#include "utils/Window.h"

#include <glm/gtc/type_ptr.hpp>

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

	KeyState Window::key_state(int key) {
		return static_cast<KeyState>(glfwGetKey(m_raw_window, key));
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
} // namespace utils