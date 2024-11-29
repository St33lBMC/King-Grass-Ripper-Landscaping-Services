#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <utility>
#include <vector>

#include "utils/Verify.h"

namespace ecs::archetypal {

	/// A generational arena allocator data structure.
	template<typename T, typename Index = uint32_t, typename GenerationCount = uint32_t> class Arena {
			static_assert(std::is_unsigned_v<Index>, "index must be unsigned");
			static_assert(std::is_unsigned_v<GenerationCount>, "generation count must be unsigned");

			struct ArenaSlot {
					Index next_free = 0;
					GenerationCount generation = 0;
					alignas(T) uint8_t data[sizeof(T)];

					ArenaSlot(T&& v) : next_free(0), generation(0) {
						set(std::forward<T&&>(v));
					}

					T* operator*() {
						return reinterpret_cast<T*>(data);
					}

					void set(T&& v) {
						***this = std::move(v);
					}
			};

			std::vector<ArenaSlot> m_data;

			Index next_free = 0;

		public:
			struct ArenaIndex {
					Index index;
					GenerationCount generation;

					ArenaIndex(Index index, GenerationCount generation) : index(index), generation(generation) {}
			};

			ArenaSlot& slot_for(ArenaIndex index) {
				auto& slot = m_data[index.index];
				if (index.generation != slot.generation) {
					PANIC("generational mismatch");
				}
				return slot;
			}

			ArenaIndex add(T&& value) {
				if (next_free == 0) {
					auto index = m_data.size();
					ArenaSlot slot = ArenaSlot(std::forward<T&&>(value));
					m_data.push_back(slot);
					return ArenaIndex(index, 0);
				} else {
					auto index = next_free - 1;
					auto& slot = m_data[index];
					slot.generation++;
					slot.set(std::forward<T&&>(value));
					next_free = slot.next_free;
					slot.next_free = 0;
					return ArenaIndex(index, slot.generation);
				}
			}

			T remove(ArenaIndex index) {
				auto& slot = slot_for(index);
				slot.next_free = next_free;
				next_free = index.index + 1;
				return **slot;
			}

			T& get(ArenaIndex index) {
				auto& slot = slot_for(index);
				return **slot;
			}

			~Arena() {
				if constexpr (!std::is_trivially_destructible_v<T>) {
					for (ArenaSlot& slot : m_data) {
						if (slot.next_free == 0) {
							(*slot)->~T();
						}
					}
				}
			}
	};

}; // namespace ecs::archetypal
