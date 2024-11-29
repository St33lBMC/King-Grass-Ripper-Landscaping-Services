#include "ecs/archetypal/TypeSet.h"

#include <algorithm>
#include <cstring>

#include "utils/Verify.h"

namespace ecs::archetypal {
	size_t TypeSet::index_of(std::type_index const& target) const {
		size_t current_index = 0;
		for (auto contained_type : types()) {
			if (contained_type == target) {
				return current_index;
			}
			current_index++;
		}
		PANIC("type not found in typekey");
	}

	bool TypeSet::contains(std::type_index target_type) const {
		for (auto contained_type : types()) {
			if (contained_type == target_type) {
				return true;
			}
		}
		return false;
	}

	bool TypeSet::is_subset_of(const TypeSet& rhs) const {
		for (auto contained_type : types()) {
			if (!rhs.contains(contained_type))
				return false;
		}
		return true;
	}

	bool TypeSet::operator==(const TypeSet& rhs) const {
		if (m_num_contained_types != rhs.m_num_contained_types)
			return false;

		auto lhs_types = types();
		auto rhs_types = rhs.types();

		bool out = true;
		for (size_t i = 0; i < m_num_contained_types; i++) {
			if (lhs_types[i] != rhs_types[i])
				out = false;
		}
		return out;
	}

}; // namespace ecs::archetypal
