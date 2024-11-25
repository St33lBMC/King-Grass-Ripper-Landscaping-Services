#pragma once
#include <glm/ext.hpp>
#include <string_view>
#include <vector>

#include "graphics/Model.h"
#include "objloader.h"

namespace models {
	class ObjectModel {
			std::vector<glm::vec3> m_vertices;
			std::vector<glm::vec2> m_uvs;
			std::vector<glm::vec3> m_normals;

		public:
			ObjectModel(std::string_view path) {
				loadOBJ(path.data(), m_vertices, m_uvs, m_normals);
			}

			void upload_to(graphics::Model& model) {
				model.vertices().upload_data(m_vertices, UsageHint::Static);
				model.normals().upload_data(m_normals, UsageHint::Static);
				model.uvs().upload_data(m_uvs, UsageHint::Static);
			}
	};
} // namespace models
