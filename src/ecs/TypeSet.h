#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <typeindex>

#include "utils/Verify.h"

namespace ecs {
	/// Maximum number of types. Chosen to make
	/// TypeSet exactly 512 bytes.
	constexpr static size_t MAX_TYPES = 63;

	/// An ordered set of types. Construction is
	/// order-independent.
	class TypeSet {
			/// The backing storage for `MAX_TYPES` types.
			alignas(std::type_index) uint8_t m_contained_types[MAX_TYPES * sizeof(std::type_index)];
			/// The number of types actually contained within this set.
			size_t m_num_contained_types;

			constexpr TypeSet(std::span<const std::type_index> data) : m_num_contained_types(data.size()) {
				std::memcpy(m_contained_types, data.data(), data.size_bytes());
			}

		public:
			/// Returns the index of a type within this set.
			size_t index_of(std::type_index const& target);

			size_t size() {
				return m_num_contained_types;
			}

			/// Returns a span to the contained types, not
			/// the whole backing storage.
			std::span<const std::type_index> types() const {
				return std::span(reinterpret_cast<std::type_index const*>(m_contained_types), m_num_contained_types);
			}

			std::span<std::type_index> types() {
				return std::span(reinterpret_cast<std::type_index*>(m_contained_types), m_num_contained_types);
			}

			/// Returns whether or not this set
			/// contains a specific type.
			bool contains(std::type_index cmp) const;

			template<typename T> inline bool contains() const {
				return contains(typeid(T));
			}

			/// Returns whether this set is a subset of `rhs`,
			/// e.g. does `rhs` contain every type this set does.
			bool is_subset_of(const TypeSet& rhs) const;

			bool operator==(const TypeSet& rhs) const;

			/// Creates a new TypeSet from a parameter pack.
			/// Order-independent.
			template<typename... T> static constexpr TypeSet create() {
				constexpr auto num_types = sizeof...(T);
				static_assert(num_types <= 63, "too many archetype values");
				std::type_index args[] = {std::type_index(typeid(T))...};
				std::sort(args, args + num_types);

				return TypeSet(std::span(args, num_types));
			}

			void add(std::type_index addition) {
				if (m_num_contained_types >= MAX_TYPES) {
					PANIC("too many types. cannot add");
				}
				m_num_contained_types++;
				types()[m_num_contained_types - 1] = addition;
			}
	};
}; // namespace ecs
