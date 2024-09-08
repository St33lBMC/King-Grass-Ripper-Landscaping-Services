#include "utils/Movement.h"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "utils/Camera.h"
#include "utils/Window.h"

namespace utils {

	void Movement::process_movement(Window& window, Camera& camera) {
		if (window.key_state(GLFW_KEY_S) == KeyState::Pressed) {
			camera.m_position -= m_speed * camera.forward();
		}
		if (window.key_state(GLFW_KEY_W) == KeyState::Pressed) {
			camera.m_position += m_speed * camera.forward();
		}
		if (window.key_state(GLFW_KEY_A) == KeyState::Pressed) {
			camera.m_position += m_speed * camera.right();
		}
		if (window.key_state(GLFW_KEY_D) == KeyState::Pressed) {
			camera.m_position -= m_speed * camera.right();
		}

		auto new_cursor_pos = window.cursor_position();

		auto cursor_delta = new_cursor_pos - m_last_cursor_pos;

		m_last_cursor_pos = new_cursor_pos;

		camera.m_pitch += cursor_delta.y * 0.1;
		camera.m_yaw += cursor_delta.x * 0.1;

		camera.m_pitch = glm::min(camera.m_pitch, 90.0f);
		camera.m_pitch = glm::max(camera.m_pitch, -90.0f);
	}
} // namespace utils