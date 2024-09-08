#include "Game.h"

void Game::loop() {
	do {
		m_movement.process_movement(m_window, m_camera);
		m_shader_program.set_uniform(
			"model_matrix",
			glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))
		);
		m_shader_program.set_uniform("view_matrix", m_camera.view());

		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw model(s)
		drawModel();

		// Swap buffers
		m_window.swap_buffers();
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (m_window.key_state(GLFW_KEY_ESCAPE) != KeyState::Pressed && !m_window.should_close());
}

Game::Game(Window&& window, std::unique_ptr<AssetProvider> asset_provider) :
	m_camera(glm::vec3(0, 0, 0)),
	m_window(std::move(window)),
	m_asset_manager(std::move(asset_provider)) {
	auto vertex_shader = Shader(ShaderType::Vertex);
	auto fragment_shader = Shader(ShaderType::Fragment);

	auto vs = m_asset_manager.get_asset<types::TextAsset>("shaders/shader.vs");

	vertex_shader.upload_shader_source(vs->text());

	vertex_shader.compile_shader();

	auto fs = m_asset_manager.get_asset<types::TextAsset>("shaders/shader.fs");

	fragment_shader.upload_shader_source(fs->text());

	fragment_shader.compile_shader();

	m_shader_program.link([&](Linking& l) {
		l.attach_shader(vertex_shader);
		l.attach_shader(fragment_shader);

		l.bind_attribute_location("position", 0);
	});

	m_shader_program.use_program();

	glm::mat4 model_matrix = glm::mat4(1.0);
	m_shader_program.set_uniform("model_matrix", model_matrix);
	m_shader_program.set_uniform("projection_matrix", m_camera.perspective());
}
