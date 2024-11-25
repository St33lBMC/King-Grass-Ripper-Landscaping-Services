#pragma once
#include <fmt/core.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <span>
#include <typeindex>
#include <vector>

#include "ecs/TypeSet.h"
#include "utils/Verify.h"

namespace ecs {

	template<typename T, typename... Ts> struct Index;

	template<typename T, typename... Ts> struct Index<T, T, Ts...>: std::integral_constant<std::size_t, 0> {};

	template<typename T, typename U, typename... Ts> struct Index<T, U, Ts...>:
		std::integral_constant<std::size_t, 1 + Index<T, Ts...>::value> {};

	template<typename... T> class ArchetypeView {
			friend class Archetype;

			TypeSet m_key;

			std::array<void*, sizeof...(T)> m_accessors;
			size_t m_num_values;

			ArchetypeView(size_t num_values, std::array<void*, sizeof...(T)> accessors) :
				m_key(TypeSet::create<T...>()),
				m_accessors(accessors),
				m_num_values(num_values) {}

		public:
			template<typename A> A& get(size_t index) {
				VERIFY(index < m_num_values, "out-of-bounds access");
				auto type_index = Index<A, T...>::value;
				fmt::print("Type index {}\n", type_index);
				return reinterpret_cast<A*>(m_accessors[type_index])[index];
			}

			size_t num_values() {
				return m_num_values;
			}
	};

	class Archetype {
			friend class World;
			using SlotIndex = size_t;

			TypeSet m_key;
			std::array<void*, MAX_TYPES> m_data;
			size_t m_num_stored;

			Archetype(TypeSet key) : m_key(key) {}

		public:
			template<typename... T> static Archetype create() {
				Archetype a(TypeSet::create<T...>());
				a.m_num_stored = 0;
				return a;
			}

			template<typename... T> ArchetypeView<T...> view() {
				auto view_key = TypeSet::create<T...>();
				VERIFY(view_key.is_subset_of(m_key), "bad view");

				std::array<void*, sizeof...(T)> accessors;
				size_t i = 0;

				std::type_index array[] {std::type_index(typeid(T))...};

				for (size_t iter = 0; iter < sizeof...(T); iter++) {
					auto t = array[iter];
					size_t index = m_key.index_of(t);
					fmt::print("thing : in slot {} is {}\n", i, t.name());
					accessors[i++] = m_data[index];
				}

				return ArchetypeView<T...>(m_num_stored, accessors);
			}

			template<typename... T> SlotIndex add(T const&&... values) {
				auto key = TypeSet::create<T...>();
				VERIFY(key == m_key, "bad set of values");
				// TODO ordering is UB rnlol
				auto prev_stored = m_num_stored++;

				(
					[&] {
						auto value = values;

						size_t data_slot = key.index_of(typeid(decltype(value)));

						decltype(value)* new_ptr =
							reinterpret_cast<decltype(value)*>(malloc(sizeof(decltype(value)) * m_num_stored));

						if (prev_stored > 0) {
							void* old_ptr = m_data[data_slot];
							memcpy(new_ptr, old_ptr, sizeof(decltype(value)) * prev_stored);
							free(reinterpret_cast<decltype(value)*>(old_ptr));
						}
						fmt::print("agh\n");
						new_ptr[m_num_stored - 1] = std::move(value);
						m_data[data_slot] = new_ptr;
						fmt::print("bah\n");
						data_slot++;
					}(),
					...
				);
				return prev_stored;
			}
	};

	class World {
			std::vector<Archetype> m_archetypes;

		public:
			World(std::vector<Archetype> archetypes) : m_archetypes(archetypes) {}

			template<typename... Ts, typename Query> void query(Query query) {
				TypeSet key = TypeSet::create<Ts...>();
				for (auto archetype : m_archetypes) {
					if (key.is_subset_of(archetype.m_key)) {
						ArchetypeView<Ts...> view = archetype.view<Ts...>();

						for (size_t index = 0; index < view.num_values(); index++) {
							query((std::forward<Ts&>(view.template get<Ts>(index)), ...));
						}
					}
				}
			}
	};

} // namespace ecs
