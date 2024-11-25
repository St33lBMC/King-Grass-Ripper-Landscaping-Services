#include "ecs/TypeSet.h"

#include "catch2/catch_test_macros.hpp"

TEST_CASE("typeset order dependence", "[TypeSet]") {
	ecs::TypeSet set = ecs::TypeSet::create<int, long, short>();
	ecs::TypeSet set2 = ecs::TypeSet::create<long, short, int>();

	REQUIRE(set == set2);
	REQUIRE(set.is_subset_of(set2));
	REQUIRE(set2.is_subset_of(set));
}

TEST_CASE("typeset inequality", "[TypeSet]") {
	ecs::TypeSet set = ecs::TypeSet::create<int, long, short>();
	ecs::TypeSet set2 = ecs::TypeSet::create<long, short>();

	REQUIRE(set != set2);
	REQUIRE(!set.is_subset_of(set2));
	REQUIRE(set2.is_subset_of(set));
}

TEST_CASE("typeset indexing", "[TypeSet]") {
	ecs::TypeSet set = ecs::TypeSet::create<int, long, short>();
	ecs::TypeSet set2 = ecs::TypeSet::create<long, short, int>();

	REQUIRE(set.index_of(typeid(int)) == set2.index_of(typeid(int)));
	REQUIRE(set.index_of(typeid(long)) == set2.index_of(typeid(long)));
	REQUIRE(set.index_of(typeid(short)) == set2.index_of(typeid(short)));
}
