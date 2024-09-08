#ifndef OBJMODEL_H
#define OBJMODEL_H
#include <GL/glew.h>

#include <string>
#include <vector>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

#include "gl_wrapper/buffer/Buffer.h"

class ObjectModel {
	public:
		ObjectModel(const char path[], bool normalized);
        ~ObjectModel();
		void drawModel();

	private:
		//dick
		GLuint vao;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
};

extern GLuint vao;
#endif