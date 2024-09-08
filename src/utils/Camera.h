#pragma once

#include <glm/ext.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace utils {

	class Camera {
		public:
			float m_fov = 35.0f;
			float m_aspect = 1.0;
			float m_pitch = 0.0;
			float m_yaw = 0.0;
			glm::vec3 m_position;

			explicit Camera(glm::vec3 position) : m_position(position) {}

			/// A direction vector representing forwards, from
			/// the POV of this camera.
			glm::vec3 forward();

			/// A direction vector representing right, from
			/// the POV of this camera.
			glm::vec3 right();

			/// A direction vector representing upwards, from
			/// the POV of this camera.
			glm::vec3 up();

			/// The view matrix of this camera.
			glm::mat4 view();

			/// The perspective matrix of this camera.
			glm::mat4 perspective();
	};
}; // namespace utils