#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

namespace utils {
	class Window;
	class Camera;

	class Movement {
			glm::dvec2 m_last_cursor_pos;
			bool read_cursor_pos = true;

		public:
			float m_speed = 0.1;

			/// Moves `camera` based on input from `window`.
			void process_movement(Window& window, Camera& camera);
	};
} // namespace utils