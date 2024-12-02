#include "ecs/World.h"

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"
#include "ecs/Query.h"

using namespace ecs::archetypal;

TEST_CASE("world query", "[ecs][World]") {
	{
		ecs::World world;

		world.add((int)9, (long)5);
		world.add((int)10);
		int output = 0;
		world.add_system<ecs::Query<ecs::Component<int&>>>([&output](auto&, auto world_view, auto query) {
			world_view.satisfy(query, [&output](int& integer) { output += integer; });
		});
		world.run();
		REQUIRE(output == 19);
		for (int i = 0; i < 10; i++) {
			world.add((int)9, (long)i);
		}
		BENCHMARK("10 queries") {
			for (int i = 0; i < 10; i++) {
				world.run();
			}
		};
	}
}
