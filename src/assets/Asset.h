#pragma once

#include <istream>
#include <type_traits>
namespace assets {

    class Asset {
        public:
            virtual ~Asset() {};
    };

	template<typename T>
	concept Assetable = requires(std::istream* stream) {
        { T(stream) };
        { std::is_base_of<T, Asset>::value };
	};
}