#pragma once

#include <cstddef>

#include "utils/Verify.h"

namespace ecs {
	enum class BorrowState { None, Shared, Unique };

	inline bool can_borrow_with(BorrowState target, BorrowState request) {
		if (target == BorrowState::None)
			return true;

		if (target == BorrowState::Unique)
			return false;

		// Can always borrow a shared reference as shared.
		if (target == BorrowState::Shared && request == BorrowState::Shared)
			return true;

		return false;
	}

	class BorrowTracker {
			/// The borrow state of this tracker.
			BorrowState m_state = BorrowState::None;
			/// The number of shared borrows.
			std::size_t m_borrow_count = 0;

		public:
			BorrowState state() const {
				return m_state;
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

} // namespace ecs
