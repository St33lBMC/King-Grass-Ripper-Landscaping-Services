#pragma once
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>

namespace utils {
	enum class KeyState : int { Pressed = GLFW_PRESS, Released = GLFW_RELEASE };

	class Window {
			GLFWwindow* m_raw_window;

		public:
			/// Creates a new Window from a raw
			/// GLFW window.
			explicit Window(GLFWwindow* window) : m_raw_window(window) {}

			~Window();

			Window(Window&& w);

			/// Returns a pointer to the raw window
			/// if this breaks your encapsulation fix it bucko
			GLFWwindow* get_raw_window() {
				return m_raw_window;
			}

			/// Swaps the buffers. Corresponds to
			/// `glfwSwapBuffers(window);`
			void swap_buffers();

			/// Gets the state of key `key`. Corresponds
			/// to `glfwGetKey(window, key);`
			KeyState key_state(int key);

			/// Checks if this window should close.
			/// Corresponds to `glfwWindowShouldClose(window);`
			bool should_close();

			/// Gets the current cursor position.
			/// Corresponds to `glfwGetCursorPos`.
			glm::dvec2 cursor_position();
	};
} // namespace utils