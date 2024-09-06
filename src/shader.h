#ifndef SHADER_HPP
#define SHADER_HPP
#include <GL/glew.h>
// borrowed from https://github.com/opengl-tutorials/ogl/blob/master/common/shader.hpp
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif