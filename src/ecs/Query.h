#pragma once
#include <fmt/core.h>

#include <bitset>
#include <type_traits>

#include "ecs/BorrowState.h"
#include "ecs/TypeSet.h"

namespace ecs {

	/// Marker type for use in a `Query`.
	/// Const references are shared, normal references are taken uniquely.
	template<typename T> struct Component {
			static_assert(std::is_reference<T>::value, "must be reference");

			static constexpr bool UNIQUE = !std::is_const_v<std::remove_reference_t<T>>;
			using Type = std::remove_reference_t<T>;
			using RefType = T;
	};

	template<typename T> struct Underlying {
			using Type = void;
			using RefType = void;
	};

	template<typename T> struct Underlying<Component<T>> {
			using Type = typename Component<T>::Type;
			using RefType = typename Component<T>::RefType;
	};

	template<typename T> struct Badge {};

	/// Represents a query.
	template<typename... T> struct Query {
			static_assert(sizeof...(T) < MAX_TYPES, "too many types");

			/// A bitset, one bit for each type, declaring
			/// whether or not it must be accessed uniquely.
			std::bitset<MAX_TYPES> m_unique_set;

			/// The types this query contains.
			TypeSet m_contained_types;

			/// Handler for components.
			template<typename C> void inner(Badge<Component<C>>, size_t index) {
				m_unique_set[index] = Component<C>::UNIQUE;
				m_contained_types.add(typeid(typeof(typename Component<C>::Type)));
			}

			/// Fail compilation if any part of the query is not
			/// in a recognised wrapper type.
			template<typename C> void inner(Badge<C>, size_t) = delete;

			Query() : m_contained_types(TypeSet::create()) {
				size_t index = 0;
				(inner(Badge<T>(), index++), ...);
			}

			BorrowState borrow(size_t index) {
				if (m_unique_set[index])
					return BorrowState::Unique;
				else
					return BorrowState::Shared;
			}
	};

} // namespace ecs