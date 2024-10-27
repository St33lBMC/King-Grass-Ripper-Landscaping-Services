#include "utils/Movement.h"

#include <GLFW/glfw3.h>

#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "utils/Camera.h"
#include "utils/Window.h"

namespace utils {

	void Movement::process_movement(Window& window, Camera& camera) {
		if (window.key_state(GLFW_KEY_S) == ButtonState::Pressed) {
			camera.m_position -= m_speed * camera.forward();
		}
		if (window.key_state(GLFW_KEY_W) == ButtonState::Pressed) {
			camera.m_position += m_speed * camera.forward();
		}
		if (window.key_state(GLFW_KEY_A) == ButtonState::Pressed) {
			camera.m_position += m_speed * camera.right();
		}
		if (window.key_state(GLFW_KEY_D) == ButtonState::Pressed) {
			camera.m_position -= m_speed * camera.right();
		}
		if (window.key_state(GLFW_KEY_ESCAPE) == ButtonState::Pressed) {
			//unlock cursor
			window.set_cursor_mode(CursorMode::Normal);
			m_last_cursor_pos = window.cursor_position();
			read_cursor_pos = false;
		}
		if (window.mouse_button_state(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed) {
			//relock cursor
			window.set_cursor_mode(CursorMode::Disabled);
			window.cursor_position(m_last_cursor_pos);
			read_cursor_pos = true;
		}

		auto new_cursor_pos = window.cursor_position();

		glm::dvec2 cursor_delta = glm::dvec2(0.0, 0.0);

		// only update viewport based on cursor position if the cursor is locked
		if (read_cursor_pos) {
			cursor_delta = new_cursor_pos - m_last_cursor_pos;
			m_last_cursor_pos = new_cursor_pos;
		}

		camera.m_pitch += cursor_delta.y * 0.1;
		camera.m_yaw += cursor_delta.x * 0.1;

		camera.m_pitch = glm::min(camera.m_pitch, 90.0f);
		camera.m_pitch = glm::max(camera.m_pitch, -90.0f);
	}
} // namespace utils