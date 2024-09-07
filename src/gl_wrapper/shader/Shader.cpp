#include "gl_wrapper/shader/Shader.h"

namespace gl_wrapper::shader {

	void Shader::compile_shader() {
		glCompileShader(m_raw_id);

		switch (get_info(ShaderIV::CompileStatus)) {
			case GL_TRUE:
				return;
			default:
				throw ShaderException(info_log());
		}
	}

	void Shader::upload_shader_source(std::string_view source) {
		GLint lengths[1];
		lengths[0] = source.size();

		const char* sources[1];
		sources[0] = source.data();

		glShaderSource(m_raw_id, 1, sources, lengths);
	}

	std::string Shader::info_log() {
		auto len = get_info(ShaderIV::InfoLogLen);
		std::string output;
		output.resize(len);

		glGetShaderInfoLog(m_raw_id, len, nullptr, output.data());

		return output;
	}

	int32_t Shader::get_info(ShaderIV var) {
		int32_t status = 0;
		glGetShaderiv(m_raw_id, (GLenum)var, &status);
		return status;
	}

	std::string Program::info_log() {
		auto len = get_info(ProgramIV::InfoLogLen);
		std::string output;
		output.resize(len);

		glGetProgramInfoLog(m_raw_id, len, nullptr, output.data());

		return output;
	}

	int32_t Program::get_info(ProgramIV var) {
		int32_t status = 0;
		glGetProgramiv(m_raw_id, (GLenum)var, &status);
		return status;
	}

	GLint Program::get_uniform_location(std::string_view location) {
		return glGetUniformLocation(m_raw_id, location.data());
	}

	void Linking::link() {
		glLinkProgram(m_program->raw_id());

		switch (m_program->get_info(ProgramIV::LinkStatus)) {
			case GL_TRUE:
				return;
			default:
				throw ShaderException(m_program->info_log());
		}
	}

	void Linking::bind_attribute_location(
		std::string_view attribute,
		GLuint location
	) {
		glBindAttribLocation(m_program->raw_id(), location, attribute.data());
	}

	void Linking::attach_shader(Shader& shader) {
		glAttachShader(m_program->raw_id(), shader.raw_id());
		m_attached_shaders.push_back(shader.raw_id());
	}

	Linking::~Linking() {
		for (auto shader : m_attached_shaders) {
			glDetachShader(m_program->raw_id(), shader);
		}
	}
}; // namespace gl_wrapper::shader
