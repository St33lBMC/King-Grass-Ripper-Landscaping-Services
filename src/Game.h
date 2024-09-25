#pragma once

#include <glm/fwd.hpp>
#include <memory>

#include "gl_wrapper/shader/Shader.h"
#include "graphics/Model.h"
#include "models/ObjectModel.h"
#include "utils/Camera.h"
#include "utils/Movement.h"
#include "utils/Window.h"

using namespace utils;
using namespace gl_wrapper::shader;

class Game {
		Camera m_camera;
		Program m_shader_program;
		Window m_window;
		Movement m_movement;

	public:
		Game(Window&& window);

		Camera& camera() {
			return m_camera;
		}

		/// Runs the game loop.
		void loop(std::vector<graphics::Model>);
};