#pragma once
#include <functional>
#include <type_traits>

#include "ecs/CommandQueue.h"

namespace ecs {

	// class System {

	// 	std::function<void(World&)> m_internal_func;

	// 	System(auto func) : m_internal_func(func) {}

	// 	public:
	// 		template<typename... SystemParameters>
	// 		System create(std::function<void(CommandQueue&, typeof(typename SystemParameters::SystemType)...)> func) {
	// 			return System([func](World& world){
	// 				func(nullptr, 4);
	// 			});
	// 		}

	// };

}; // namespace ecs
