#include "ecs/archetypal/Archetype.h"

#include <unordered_set>
#include <utility>
#include <vector>

#include "catch2/catch_test_macros.hpp"
#include "ecs/archetypal/TypeSet.h"
#include "utils/Verify.h"

using namespace ecs::archetypal;

// TEST_CASE("archetype query", "[ecs][Archetype]") {
// 	Archetype a = Archetype::create<int, long>();
// 	a.add(4, 5l);
// 	a.add(6l, 7);
// 	a.add(9l, 21);

// 	ecs::Query<ecs::Component<int&>, ecs::Component<long const&>> q;

// 	std::unordered_set<int> ints;
// 	std::unordered_set<long> longs;

// 	a.satisfy(q, [&ints, &longs](int& integer, long const& longerger) {
// 		ints.insert(integer);
// 		longs.insert(longerger);
// 	});

// 	REQUIRE(ints.contains(4));
// 	REQUIRE(ints.contains(7));
// 	REQUIRE(ints.contains(21));

// 	REQUIRE(longs.contains(9l));
// 	REQUIRE(longs.contains(6l));
// 	REQUIRE(longs.contains(5l));
// }

// TEST_CASE("archetype query subset", "[ecs][Archetype]") {
// 	Archetype a = Archetype::create<int, long>();
// 	a.add(4, 5l);
// 	a.add(6l, 7);
// 	a.add(9l, 21);

// 	ecs::Query<ecs::Component<int&>> q;

// 	std::unordered_set<int> ints;

// 	a.satisfy(q, [&ints](int& integer) { ints.insert(integer); });

// 	REQUIRE(ints.contains(4));
// 	REQUIRE(ints.contains(7));
// 	REQUIRE(ints.contains(21));
// }

// TEST_CASE("archetype unsatisfiable query", "[ecs][Archetype][!shouldfail]") {
// 	Archetype a = Archetype::create<long>();

// 	ecs::Query<ecs::Component<int&>> q;

// 	a.satisfy(q, [](int&) { (void)0; });
// }

TEST_CASE("archetype destructors", "[ecs][Archetype]") {
	struct Cool {
			int* target;

			Cool(int* b) : target(b) {}

			~Cool() {
				(*target) = (*target) + 1;
			}
	};

	int counter = 0;
	Cool c = Cool(&counter);
	{
		Archetype a = Archetype::create<Cool>();
		a.add((Cool &&) c);
		a.add((Cool &&) c);
		a.add((Cool &&) c);
		a.add((Cool &&) c);
		a.add((Cool &&) c);
	}
	REQUIRE(counter == 5);
}
