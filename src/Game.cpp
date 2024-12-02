#include "Game.h"

#include <fmt/core.h>

#include <fstream>
#include <glm/fwd.hpp>

#include "ecs/Query.h"
#include "ecs/World.h"
#include "graphics/Model.h"
#include "models/ObjectModel.h"
#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"

void Game::loop() {
	do {
		FrameMark;
		m_movement.process_movement(m_window, m_camera);
		m_shader_program.set_uniform("view_matrix", m_camera.view());

		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_world.run();

		// Swap buffers
		m_window.swap_buffers();
		TracyGpuCollect;
		glfwPollEvents();

	} // Check if the window was closed
	// escape key functionality changed to unlocking cursor
	while (!m_window.should_close());
}

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	PANIC("read of {} failed: {}", filename, std::strerror(errno));
}

Game::Game(Window&& window, ecs::World&& world) :
	m_camera(glm::vec3(0, 0, 0)),
	m_window(std::move(window)),
	m_world(std::move(world)) {
	auto vertex_shader = Shader(ShaderType::Vertex);
	auto fragment_shader = Shader(ShaderType::Fragment);

	vertex_shader.upload_shader_source(get_file_contents("../assets/shaders/shader.vs"));

	vertex_shader.compile_shader();

	fragment_shader.upload_shader_source(get_file_contents("../assets/shaders/shader.fs"));

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

	m_world.add_system<ecs::Query<ecs::Component<glm::vec3 const&>, ecs::Component<graphics::Model const&>>>(
		[this](
			ecs::CommandQueue&,
			ecs::WorldView world,
			ecs::Query<ecs::Component<glm::vec3 const&>, ecs::Component<graphics::Model const&>> query
		) {
			world.satisfy(query, [this](glm::vec3 const& position, graphics::Model const& model) {
				//fmt::print("this does indeed have side effects\n");
				model.draw(m_shader_program, glm::translate(glm::mat4(1.0f), position));
			});
		}
	);
}
