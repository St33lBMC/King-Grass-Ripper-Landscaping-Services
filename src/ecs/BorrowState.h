#pragma once

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

} // namespace ecs
