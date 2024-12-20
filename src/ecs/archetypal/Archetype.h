#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

#include "ecs/Query.h"
#include "ecs/archetypal/BorrowState.h"
#include "ecs/archetypal/TypeSet.h"
#include "utils/Verify.h"

namespace ecs::archetypal {

	template<class T> void destruct(void* x) {
		reinterpret_cast<const T*>(x)->~T();
	}

	class Archetype {
			struct ComponentStore {
					/// Type-erased pointer.
					void* m_data_ptr = nullptr;
					/// The borrow state of this component.
					BorrowState m_state = BorrowState::None;
					/// The number of shared borrows.
					size_t m_borrow_count = 0;
					/// The stride of this type.
					size_t m_stride;
					/// This type's destructor.
					void (*m_destructor)(void*) = nullptr;

					ComponentStore(size_t stride = 0) : m_stride(stride) {}

					template<typename T> static ComponentStore create() {
						ComponentStore store(std::max(alignof(T), sizeof(T)));
						store.m_data_ptr = nullptr;
						if constexpr (std::is_trivially_destructible_v<T>) {
							store.m_destructor = nullptr;
						} else {
							store.m_destructor = &destruct<T>;
						}
						return store;
					}

					void borrow_as(BorrowState state);

					void release_borrow();
			};

			friend class World;

			TypeSet m_contained_types;
			std::array<ComponentStore, MAX_TYPES> m_data;
			uint8_t m_num_stored = 0;

			Archetype(TypeSet key) : m_contained_types(key) {}

			void set_data(size_t index, ComponentStore c) {
				m_data[index] = c;
			}

			template<typename T> T& access(size_t index) {
				VERIFY(m_contained_types.contains<T>(), "trying to access type that is not contained");
				VERIFY(index < m_num_stored, "index out of bounds");
				return reinterpret_cast<T*>(m_data[m_contained_types.index_of(typeid(T))].m_data_ptr)[index];
			}

			template<typename T, typename Handler> void inner_handler(Badge<T>, size_t, Handler&) = delete;

			template<typename T> T inner_handler(Badge<Component<T>>, size_t index) {
				return (std::forward<T>(access<typename Component<T>::Type>(index)));
			}

		public:
			using SlotIndex = size_t;

			Archetype(Archetype const& other) = delete;
			Archetype& operator=(Archetype&) = delete;

			Archetype(Archetype&& other) : m_contained_types(other.m_contained_types) {
				m_data = other.m_data;
				m_num_stored = other.m_num_stored;
				other.m_num_stored = 0;
				other.m_contained_types = TypeSet::empty();
			}

			template<typename... T, typename Handler> void satisfy(Query<T...> query, Handler handler) {
				VERIFY(query.m_contained_types.is_subset_of(m_contained_types), "Bad query");

				for (size_t i = 0; i < query.m_contained_types.size(); i++) {
					m_data[i].borrow_as(query.borrow(i));
				}

				for (size_t i = 0; i < m_num_stored; i++) {
					handler(access<typename Underlying<T>::Type>(i)...);
				}

				for (size_t i = 0; i < query.m_contained_types.size(); i++) {
					m_data[i].release_borrow();
				}
			}

			template<typename... T> static Archetype create() {
				Archetype a(TypeSet::create<T...>());
				a.m_num_stored = 0;
				(a.set_data(a.m_contained_types.index_of(typeid(T)), ComponentStore::create<T>()), ...);
				return a;
			}

			template<typename... T> SlotIndex add(T&&... values) {
				auto types = TypeSet::create<T...>();
				VERIFY(types == m_contained_types, "trying to add with wrong types");
				auto prev_stored = m_num_stored++;
				// For every type we are trying to add,
				(
					[&] {
						auto& value = values;

						// grab its defined index
						size_t data_slot = m_contained_types.index_of(typeid(T));

						// check that it is not uniquely borrowed
						if (m_data[data_slot].m_state == BorrowState::Unique) {
							PANIC("trying to mutate uniquely-borrowed component");
						}

						// allocate new memory for it
						T* new_ptr = reinterpret_cast<T*>(aligned_alloc(alignof(T), sizeof(T) * m_num_stored));

						if (prev_stored > 0) {
							// deallocate the old memory, if necessary
							void* old_ptr = m_data[data_slot].m_data_ptr;
							memcpy((void*)new_ptr, old_ptr, sizeof(T) * prev_stored);
							free(reinterpret_cast<T*>(old_ptr));
						}
						new (&new_ptr[m_num_stored - 1]) T(std::move(value));
						m_data[data_slot].m_data_ptr = new_ptr;

						data_slot++;
					}(),
					...
				);
				return prev_stored;
			}

			~Archetype();
	};

} // namespace ecs::archetypal
