#include "ecs/Archetype.h"

#include <unordered_set>
#include <vector>

#include "catch2/catch_test_macros.hpp"
#include "ecs/TypeSet.h"
#include "utils/Verify.h"

TEST_CASE("archetype query", "[ecs][Archetype]") {
	ecs::Archetype a = ecs::Archetype::create<int, long>();
	a.add(4, 5l);
	a.add(6l, 7);
	a.add(9l, 21);

	ecs::Query<ecs::Component<int&>, ecs::Component<long const&>> q;

	std::unordered_set<int> ints;
	std::unordered_set<long> longs;

	a.satisfy(q, [&ints, &longs](int& integer, long const& longerger) {
		ints.insert(integer);
		longs.insert(longerger);
	});

	REQUIRE(ints.contains(4));
	REQUIRE(ints.contains(7));
	REQUIRE(ints.contains(21));

	REQUIRE(longs.contains(9l));
	REQUIRE(longs.contains(6l));
	REQUIRE(longs.contains(5l));
}

TEST_CASE("archetype unsatisfiable query", "[ecs][Archetype][!shouldfail]") {
	ecs::Archetype a = ecs::Archetype::create<long>();

	ecs::Query<ecs::Component<int&>> q;

	a.satisfy(q, [](int&) { (void)0; });
}
