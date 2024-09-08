#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include "assets/provider/AssetProvider.h"

namespace assets::provider {
	class FileAssetProvider: public AssetProvider {
			std::filesystem::path m_directory;

		public:
			explicit FileAssetProvider(std::filesystem::path directory) : m_directory(directory) {}

			virtual std::unique_ptr<std::istream> provide_asset(std::string_view identifier) const override {
				auto path = m_directory / identifier;
				auto ptr = std::make_unique<std::ifstream>(path, std::istream::in);
				return std::move(ptr);
			}

			virtual ~FileAssetProvider() {
				std::cout << "File provider destructed" << std::endl;
			};
	};
} // namespace assets::provider