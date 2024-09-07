#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "assets/Asset.h"
#include "assets/provider/AssetProvider.h"

namespace assets::manager {

	struct OpaquePtr {
			std::shared_ptr<Asset> ptr;
			std::type_index info;

			template<Assetable T> OpaquePtr(std::shared_ptr<T> value) : ptr(value), info(typeid(T)) {}

			template<Assetable T> std::shared_ptr<T> get() {
				if (typeid(T) != info) {
					throw std::bad_cast();
				}
				return std::static_pointer_cast<T>(ptr);
			}
	};

	class AssetManager {
			std::unique_ptr<provider::AssetProvider> m_asset_provider;

            /// FIXME: this cache is never cleared, even if 
            /// the pointer runs out of references.
			std::unordered_map<std::string, OpaquePtr> m_cache;

		public:
			explicit AssetManager(std::unique_ptr<provider::AssetProvider> asset_provider) : m_asset_provider(std::move(asset_provider)) {}

			template<Assetable T> std::shared_ptr<T> get_asset(std::string_view identifier) {
				auto const& cached = m_cache.find(std::string(identifier));
				if (cached != m_cache.end())
					return cached->second.get<T>();

				auto asset_stream = m_asset_provider->provide_asset(identifier);
				OpaquePtr opaque_asset(std::make_shared<T>(asset_stream.get()));

                auto shared = opaque_asset.get<T>();

				m_cache.insert(std::pair(std::string(identifier), std::move(opaque_asset)));

                return shared;
			}
	};
} // namespace assets::manager