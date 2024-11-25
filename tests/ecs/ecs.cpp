#include "catch2/catch_test_macros.hpp"
#include "ecs/World.h"

TEST_CASE("simple1") {
	ecs::Archetype archetype = ecs::Archetype::create<int, long>();
	archetype.add(5l, 4);

	auto view1 = archetype.view<int>();
	REQUIRE(view1.get<int>(0) == 4);
	auto view2 = archetype.view<long>();
	REQUIRE(view2.get<long>(0) == 5);
}
