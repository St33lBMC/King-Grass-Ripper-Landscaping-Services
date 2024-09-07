#include "utils/Camera.h"

#include <glm/ext.hpp>

namespace utils {
	glm::vec3 Camera::forward() {
		glm::vec3 direction_vector;
		direction_vector.x = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
		direction_vector.y = sin(glm::radians(m_pitch));
		direction_vector.z = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
		direction_vector = glm::normalize(direction_vector);
		return direction_vector;
	}

	glm::vec3 Camera::right() {
		return glm::vec3(
			sin(glm::radians(m_yaw) - glm::pi<float>() / 2.0),
			0.0,
			cos(glm::radians(m_yaw) - glm::pi<float>() / 2.0)
		);
	};

	glm::vec3 Camera::up() {
		return glm::cross(forward(), right());
	}

	glm::mat4 Camera::perspective() {
		return glm::perspective(glm::radians(m_fov), m_aspect, 0.0f, 10.0f);
	}

	glm::mat4 Camera::view() {
		return glm::lookAt(m_position, m_position + forward(), up());
	}
} // namespace utils