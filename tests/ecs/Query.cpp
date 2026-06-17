#include "ecs/Query.h"

#include "catch2/catch_test_macros.hpp"
#include "ecs/archetypal/TypeSet.h"

using namespace ecs::archetypal;

TEST_CASE("query construction correct", "[ecs][Query]") {
	ecs::Query<ecs::Component<int&>, ecs::Component<long const&>> q;

	REQUIRE(q.m_contained_types == TypeSet::create<int, long>());
	REQUIRE(q.m_unique_set[0]);
	REQUIRE_FALSE(q.m_unique_set[1]);
}
