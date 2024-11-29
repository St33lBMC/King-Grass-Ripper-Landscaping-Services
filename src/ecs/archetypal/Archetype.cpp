#include "ecs/archetypal/Archetype.h"

namespace ecs::archetypal {

	void Archetype::ComponentStore::borrow_as(BorrowState state) {
		VERIFY(can_borrow_with(m_state, state), "Trying to borrow when not permitted");
		m_state = state;
		if (m_state == BorrowState::Shared)
			m_borrow_count++;
	}

	void Archetype::ComponentStore::release_borrow() {
		VERIFY(m_state != BorrowState::None, "already unborrowed");
		if (m_state == BorrowState::Shared) {
			if (--m_borrow_count > 0) {
				return; // still shared borrow
			}
		}
		m_state = BorrowState::None;
	}

	Archetype::~Archetype() {
		for (size_t type_iteration = 0; type_iteration < m_contained_types.size(); type_iteration++) {
			ComponentStore& store = m_data[type_iteration];
			if (store.m_destructor != nullptr) {
				for (size_t value_iteration = 0; value_iteration < m_num_stored; value_iteration++) {
					(store.m_destructor)(&reinterpret_cast<uint8_t*>(store.m_data_ptr)[value_iteration * store.m_stride]
					);
				}
			}
			free(store.m_data_ptr);
		}
	}
}; // namespace ecs::archetypal
