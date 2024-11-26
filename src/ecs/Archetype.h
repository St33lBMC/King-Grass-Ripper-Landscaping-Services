#pragma once

#include <cstddef>
#include <cstdint>

#include "ecs/BorrowState.h"
#include "ecs/Query.h"
#include "ecs/TypeSet.h"
#include "utils/Verify.h"

namespace ecs {

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
					/// This type's destructor.
					void (*m_destructor)(void*);

					template<typename T> static ComponentStore create() {
						ComponentStore store;
						store.m_data_ptr = nullptr;
						store.m_destructor = &destruct<T>;
						return store;
					}

					void borrow_as(BorrowState state) {
						VERIFY(can_borrow_with(m_state, state), "Trying to borrow when not permitted");
						m_state = state;
						if (m_state == BorrowState::Shared)
							m_borrow_count++;
					}

					void release_borrow() {
						VERIFY(m_state != BorrowState::None, "already unborrowed");
						if (m_state == BorrowState::Shared) {
							if (--m_borrow_count > 0) {
								return; // still shared borrow
							}
						}
						m_state = BorrowState::None;
					}
			};

			friend class World;
			using SlotIndex = size_t;

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

			template<typename... T> SlotIndex add(T const&&... values) {
				auto types = TypeSet::create<T...>();
				VERIFY(types == m_contained_types, "trying to add with wrong types");
				auto prev_stored = m_num_stored++;
				// For every type we are trying to add,
				(
					[&] {
						auto value = values;

						// grab its defined index
						size_t data_slot = m_contained_types.index_of(typeid(decltype(value)));

						// check that it is not uniquely borrowed
						if (m_data[data_slot].m_state == BorrowState::Unique) {
							PANIC("trying to mutate uniquely-borrowed component");
						}

						// allocate new memory for it
						decltype(value)* new_ptr =
							reinterpret_cast<decltype(value)*>(malloc(sizeof(decltype(value)) * m_num_stored));

						if (prev_stored > 0) {
							// deallocate the old memory, if necessary
							void* old_ptr = m_data[data_slot].m_data_ptr;
							memcpy(new_ptr, old_ptr, sizeof(decltype(value)) * prev_stored);
							free(reinterpret_cast<decltype(value)*>(old_ptr));
						}
						new_ptr[m_num_stored - 1] = std::move(value);
						m_data[data_slot].m_data_ptr = new_ptr;

						data_slot++;
					}(),
					...
				);
				return prev_stored;
			}
	};

} // namespace ecs
