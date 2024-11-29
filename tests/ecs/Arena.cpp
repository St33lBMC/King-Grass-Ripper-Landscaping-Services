#include "ecs/archetypal/Arena.h"

#include "catch2/catch_test_macros.hpp"

using namespace ecs::archetypal;

TEST_CASE("arena creation", "[ecs][Arena]") {
	Arena<int> arena;
	auto index = arena.add(5);

	REQUIRE(arena.get(index) == 5);
}

TEST_CASE("arena generational tests", "[ecs][Arena]") {
	Arena<int> arena;
	auto index = arena.add(5);
	arena.remove(index);
	auto index_2 = arena.add(5);

	REQUIRE(index.index == index_2.index);
	REQUIRE(index.generation != index_2.generation);
}

TEST_CASE("arena generational check", "[ecs][Arena][!shouldfail]") {
	Arena<int> arena;
	auto index = arena.add(5);
	arena.remove(index);
	arena.add(5);

	arena.get(index);
}

TEST_CASE("arena destructors", "[ecs][Arena]") {
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
		Arena<Cool> a;
		a.add((Cool &&) c);
		a.add((Cool &&) c);
		a.add((Cool &&) c);
		a.add((Cool &&) c);
		a.add((Cool &&) c);
	}
	REQUIRE(counter == 5);
}
