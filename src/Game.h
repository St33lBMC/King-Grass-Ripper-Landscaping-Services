#pragma once

#include <glm/fwd.hpp>
#include <memory>

#include "assets/manager/AssetManager.h"
#include "assets/provider/AssetProvider.h"
#include "assets/types/TextAsset.h"
#include "gl_wrapper/shader/Shader.h"
#include "graphics/Model.h"
#include "models/ObjectModel.h"
#include "utils/Camera.h"
#include "utils/Movement.h"
#include "utils/Window.h"

using namespace utils;
using namespace gl_wrapper::shader;
using namespace assets::manager;
using namespace assets::provider;
using namespace assets;

class Game {
		Camera m_camera;
		Program m_shader_program;
		Window m_window;
		Movement m_movement;
		AssetManager m_asset_manager;

	public:
		Game(Window&& window, std::unique_ptr<AssetProvider> asset_provider);

		/// Runs the game loop.
		void loop(std::vector<graphics::Model>);
};