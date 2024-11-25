#include "ecs/Query.h"

#include "catch2/catch_test_macros.hpp"
#include "ecs/TypeSet.h"

TEST_CASE("query construction correct", "[ecs][Query]") {
	ecs::Query<ecs::Component<int&>, ecs::Component<long const&>> q;

	REQUIRE(q.m_contained_types == ecs::TypeSet::create<int, long>());
	REQUIRE(q.m_unique_set[0]);
	REQUIRE_FALSE(q.m_unique_set[1]);
}
