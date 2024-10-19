#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <memory>
#include <span>

#include "utils/Verify.h"

namespace graphics {

	class RGBA8888Image {
			size_t m_width;
			size_t m_height;
			std::unique_ptr<uint32_t[]> m_data;

			RGBA8888Image(size_t width, size_t height, std::span<uint32_t> data) :
				m_width(width),
				m_height(height),
				m_data(new uint32_t[data.size()]) {
				VERIFY(width * height == data.size());
				std::memcpy(m_data.get(), data.data(), data.size_bytes());
			}

			RGBA8888Image(size_t width, size_t height, uint32_t* data) :
				m_width(width),
				m_height(height),
				m_data(data) {}

		public:
			size_t width() {
				return m_width;
			}

			size_t height() {
				return m_height;
			}

			size_t num_pixels() {
				return m_width * m_height;
			}

			std::span<uint32_t> data() {
				return std::span(m_data.get(), num_pixels());
			}

			static RGBA8888Image from_file(std::filesystem::path path);
	};
} // namespace graphics