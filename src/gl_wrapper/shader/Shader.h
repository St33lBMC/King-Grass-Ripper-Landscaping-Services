#pragma once
#include <GL/glew.h>
#include <GL/glext.h>

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <glm/ext.hpp>

namespace gl_wrapper::shader {

	enum class ShaderType : GLenum {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

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

			char* what() {
				return m_message.data();
			}
	};

	template <typename T> void upload_uniform(T value, GLint v);
	template <> inline void upload_uniform<glm::mat4>(glm::mat4 value, GLint v) {
		glUniformMatrix4fv(v, 1, GL_FALSE, &value[0][0]);
	};

	/// Represents an OpenGL shader instance.
	class Shader {
			GLuint m_raw_id;
			bool m_moved_from = false;
		public:
			Shader(ShaderType type) {
				m_raw_id = glCreateShader(std::to_underlying(type));
			}

			~Shader() {
				if (!m_moved_from)
					glDeleteShader(m_raw_id);
			}
			Shader(Shader&& other) {
				other.m_moved_from = true;
				this->m_raw_id = other.m_raw_id;
			}

			GLuint raw_id() {
				return m_raw_id;
			}

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

			void bind_attribute_location(
				std::string_view attribute,
				GLuint location
			);

			void link();
	};

	class Program {
			GLuint m_raw_id;
			bool m_moved_from;
		public:
			Program() {
				m_raw_id = glCreateProgram();
			}

			~Program() {
				if (!m_moved_from)
					glDeleteProgram(m_raw_id);
			}

			Program(Program&& other) {
				this->m_raw_id = other.m_raw_id;
				other.m_moved_from = true;
			}

			GLuint raw_id() {
				return m_raw_id;
			}

			void use_program() {
				glUseProgram(m_raw_id);
			}

			template<typename T>
			void set_uniform(std::string_view location, T uniformable) {
				use_program();
				upload_uniform(uniformable, get_uniform_location(location));
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