#pragma once

#include <istream>
#include <memory>
#include <string_view>
namespace assets::provider {
	class AssetProvider {
        public:
            virtual std::unique_ptr<std::istream> provide_asset(std::string_view identifier) const = 0;
            virtual ~AssetProvider() = default;
    };
}; // namespace assets::provider