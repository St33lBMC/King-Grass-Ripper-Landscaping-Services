#include "ecs/archetypal/TypeSet.h"

#include "catch2/catch_test_macros.hpp"

using namespace ecs::archetypal;

TEST_CASE("typeset order dependence", "[ecs][TypeSet]") {
	TypeSet set = TypeSet::create<int, long, short>();
	TypeSet set2 = TypeSet::create<long, short, int>();

	REQUIRE(set == set2);
	REQUIRE(set.is_subset_of(set2));
	REQUIRE(set2.is_subset_of(set));
}

TEST_CASE("typeset inequality", "[ecs][TypeSet]") {
	TypeSet set = TypeSet::create<int, long, short>();
	TypeSet set2 = TypeSet::create<long, short>();

	REQUIRE(set != set2);
	REQUIRE(!set.is_subset_of(set2));
	REQUIRE(set2.is_subset_of(set));
}

TEST_CASE("typeset indexing", "[ecs][TypeSet]") {
	TypeSet set = TypeSet::create<int, long, short>();
	TypeSet set2 = TypeSet::create<long, short, int>();

	REQUIRE(set.index_of(typeid(int)) == set2.index_of(typeid(int)));
	REQUIRE(set.index_of(typeid(long)) == set2.index_of(typeid(long)));
	REQUIRE(set.index_of(typeid(short)) == set2.index_of(typeid(short)));
}
