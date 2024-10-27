#pragma once
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>

namespace utils {
	enum class ButtonState : int { Pressed = GLFW_PRESS, Released = GLFW_RELEASE };

	enum class CursorMode : int {
		Normal = GLFW_CURSOR_NORMAL,
		Hidden = GLFW_CURSOR_HIDDEN,
		Disabled = GLFW_CURSOR_DISABLED
	};

	class Window {
			GLFWwindow* m_raw_window;

		public:
			/// Creates a new Window from a raw
			/// GLFW window.
			explicit Window(GLFWwindow* window) : m_raw_window(window) {}

			~Window();

			Window(Window&& w);

			GLFWwindow* raw_window() {
				return m_raw_window;
			}

			/// Swaps the buffers. Corresponds to
			/// `glfwSwapBuffers(window);`
			void swap_buffers();

			/// Gets the state of key `key`. Corresponds
			/// to `glfwGetKey(window, key);`
			ButtonState key_state(int key);

			/// Gets the state of mouse button `button`.
			/// Corresponds to `glfwGetMouseButton(window, button);`
			ButtonState mouse_button_state(int button);

			/// Checks if this window should close.
			/// Corresponds to `glfwWindowShouldClose(window);`
			bool should_close();

			/// Gets the current cursor position.
			/// Corresponds to `glfwGetCursorPos`.
			glm::dvec2 cursor_position();

			/// Sets the current cursor position.
			/// Corresponds to `glfwSetCursorPos`.
			void cursor_position(glm::dvec2&);

			/// Sets the current cursor mode.
			/// Corresponds to `glfwSetInputMode(window, GLFW_CURSOR, mode)`.
			void set_cursor_mode(CursorMode);
	};
} // namespace utils