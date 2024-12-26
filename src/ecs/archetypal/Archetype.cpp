#include "ecs/archetypal/Archetype.h"

#include <fmt/core.h>

namespace ecs::archetypal {

	Archetype::~Archetype() {
		fmt::print("Destructing. {}\n", m_contained_types.size());
		for (size_t type_iteration = 0; type_iteration < m_contained_types.size(); type_iteration++) {
			ComponentStore& store = m_data[type_iteration];
			if (store.m_data_ptr) {
				if (store.m_destructor != nullptr) {
					for (size_t value_iteration = 0; value_iteration < m_num_stored; value_iteration++) {
						(store.m_destructor
						)(&reinterpret_cast<uint8_t*>(store.m_data_ptr)[value_iteration * store.m_stride]);
					}
				}
				free(store.m_data_ptr);
				store.m_data_ptr = nullptr;
			}
		}
	}
}; // namespace ecs::archetypal
