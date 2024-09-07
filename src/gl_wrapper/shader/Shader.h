#pragma once
#include <GL/glew.h>
#include <GL/glext.h>

#include <cstdint>
#include <exception>
#include <string>
#include <vector>

namespace gl_wrapper::Shader {

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

	template<typename T>
	concept Uniformable = requires(T t) {
		{ upload_uniform(t, GLint()) };
	};

	/// Represents an OpenGL shader instance.
	class Shader {
			GLuint m_raw_id;

		public:
			Shader(ShaderType type) {
				m_raw_id = glCreateShader((GLenum)type);
			}

			~Shader() {
				glDeleteShader(m_raw_id);
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

		public:
			Program() {
				m_raw_id = glCreateProgram();
			}

			~Program() {
				glDeleteProgram(m_raw_id);
			}

			GLuint raw_id() {
				return m_raw_id;
			}

			void use_program() {
				glUseProgram(m_raw_id);
			}

			template<Uniformable T>
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

}; // namespace gl_wrapper::Shader