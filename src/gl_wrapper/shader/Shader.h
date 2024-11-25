#pragma once
#include <GL/glew.h>

#include <cstdint>
#include <exception>
#include <glm/ext.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "gl_wrapper/Utils.h"

namespace gl_wrapper::uniforms {

	template<typename T> void upload_uniform(T& value, GLint v);

	template<> inline void upload_uniform<glm::mat4>(glm::mat4& value, GLint v) {
		glUniformMatrix4fv(v, 1, GL_FALSE, &value[0][0]);
	};

	template<> inline void upload_uniform<glm::vec4>(glm::vec4& value, GLint v) {
		glUniform4fv(v, 1, &value[0]);
	};

	template<> inline void upload_uniform<int32_t>(int32_t& value, GLint v) {
		glUniform1i(v, value);
	}
} // namespace gl_wrapper::uniforms

namespace gl_wrapper::shader {

	enum class ShaderType : GLenum { Vertex = GL_VERTEX_SHADER, Fragment = GL_FRAGMENT_SHADER };

	enum class ShaderIV : GLenum {
		ShaderType = GL_SHADER_TYPE,
		DeleteStatus = GL_DELETE_STATUS,
		CompileStatus = GL_COMPILE_STATUS,
		InfoLogLen = GL_INFO_LOG_LENGTH,
		ShaderSrcLen = GL_SHADER_SOURCE_LENGTH,
	};

	class ShaderException: public std::exception {
		private:
			std::string m_message;

		public:
			ShaderException(std::string message) : m_message(message) {}

			const char* what() const noexcept override {
				return m_message.c_str();
			}
	};

	using ShaderDestructor = decltype(GL_DEST(id) { return glDeleteShader(id); });
	using ShaderConstructor = decltype([](GLenum ty) -> GLuint { return glCreateShader(ty); });

	/// Represents an OpenGL shader instance.
	class Shader: public GLObject<Shader, ShaderConstructor, ShaderDestructor, GLenum> {
		public:
			Shader(ShaderType type) : GLObject(std::to_underlying(type)) {}

			/// Compiles this shader.
			/// Throws `ShaderCompileException` in the event of failure.
			void compile_shader();

			void upload_shader_source(std::string_view source);

			std::string info_log();

			int32_t get_info(ShaderIV var);
	};

	enum class ProgramIV : GLenum {
		LinkStatus = GL_LINK_STATUS,
		InfoLogLen = GL_INFO_LOG_LENGTH,
	};

	class Program;

	class Linking {
			friend class Program;

			Program* m_program;
			std::vector<GLuint> m_attached_shaders;

			Linking(Program* program) : m_program(program) {}

			~Linking();

		public:
			void attach_shader(Shader& shader);

			void bind_attribute_location(std::string_view attribute, GLuint location);

			void link();
	};

	using ProgramDestructor = decltype(GL_DEST(id) { return glDeleteProgram(id); });
	using ProgramConstructor = decltype([]() -> GLuint { return glCreateProgram(); });

	class Program: public GLObject<Program, ProgramConstructor, ProgramDestructor> {
		public:
			void use_program() {
				glUseProgram(m_raw_id);
			}

			template<typename T> void set_uniform(std::string_view location, T uniformable) {
				use_program();
				gl_wrapper::uniforms::upload_uniform(uniformable, get_uniform_location(location));
			}

			GLint get_uniform_location(std::string_view location);

			template<typename Function> void link(Function f) {
				Linking linking(this);

				f(linking);
				linking.link();
			}

			std::string info_log();

			int32_t get_info(ProgramIV var);
	};

}; // namespace gl_wrapper::shader

#include "UniformImpl.h"
