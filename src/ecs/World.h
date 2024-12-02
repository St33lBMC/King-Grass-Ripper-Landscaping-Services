#pragma once

#include <fmt/core.h>

#include <concepts>
#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ecs/System.h"
#include "ecs/archetypal/Archetype.h"
#include "ecs/archetypal/Arena.h"
#include "ecs/archetypal/TypeSet.h"
#include "utils/Verify.h"

namespace ecs {
	using namespace ecs::archetypal;

	class World;

	template<typename T>
	concept SystemParameter = requires(World& world) {
		{ T::value_from_world(world) } -> std::convertible_to<typename T::PassedValue>;
	};

	class WorldView {
			World& m_world;

		public:
			WorldView(World& world) : m_world(world) {}

			template<typename... T, typename Handler> void satisfy(Query<T...>& query, Handler handler);
	};

	class World {
			struct EntityEntry {
					TypeSet archetype;
					Archetype::SlotIndex index;

					EntityEntry(TypeSet archetype, Archetype::SlotIndex index) : archetype(archetype), index(index) {}
			};

			using System = std::function<void(World&)>;

			std::unordered_map<TypeSet, Archetype> m_archetypes;

			Arena<EntityEntry> m_entities;

			std::vector<System> m_systems;

			CommandQueue m_command_queue;

			using Entity = Arena<EntityEntry>::ArenaIndex;

		public:
			World() = default;
			World(World&) = delete;
			World(World&&) = default;
			World& operator=(World&) = delete;

			template<typename... Components> Entity add(Components&&... components) {
				TypeSet set = TypeSet::create<Components...>();

				auto& [_, archetype] =
					*m_archetypes.try_emplace(set, std::move(Archetype::create<Components...>())).first;

				Archetype::SlotIndex index = archetype.add(std::forward<Components&&>(components)...);

				return m_entities.add(EntityEntry(set, index));
			}

			template<SystemParameter... SystemParameters> void add_system(
				std::function<void(CommandQueue&, WorldView, typeof(typename SystemParameters::PassedValue)...)> func
			) {
				m_systems.push_back([func](World& world) {
					func(world.m_command_queue, WorldView(world), SystemParameters::value_from_world(world)...);
				});
			}

			template<typename... T, typename Handler> void satisfy(Query<T...>& query, Handler handler) {
				TypeSet set = TypeSet::create<T...>();
				for (auto& [key, archetype] : m_archetypes) {
					if (query.m_contained_types.is_subset_of(key)) {
						archetype.satisfy(query, handler);
					}
				}
			}

			void run() {
				for (System& system : m_systems) {
					(system)(*this);
				}
			}
	};

	template<typename... T, typename Handler> void WorldView::satisfy(Query<T...>& query, Handler handler) {
		m_world.satisfy(query, handler);
	}

	template<typename ResourceType> struct Resource {
			static_assert(std::is_reference_v<ResourceType>, "must be a reference");

			using PassedValue = ResourceType;

			static ResourceType value_from_world(World& world) {
				(void)world;
				PANIC("unreachable")
			}
	};

} // namespace ecs
