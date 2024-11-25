#pragma once
#include <fmt/core.h>

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <memory>
#include <span>

#include "utils/Verify.h"

namespace graphics {

	enum class PixelFormat { RGBA8888, RGB888, Grayscale8 };

	constexpr size_t bytes_per_pixel(PixelFormat t) {
		switch (t) {
			case PixelFormat::RGBA8888:
				return 4;
			case PixelFormat::RGB888:
				return 3;
			case PixelFormat::Grayscale8:
				return 1;
		}
	}

	template<PixelFormat T> class ImageRef {
			size_t m_width;
			size_t m_height;
			std::span<uint8_t> m_data;

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

			std::span<uint8_t> data() {
				return m_data;
			}

			explicit ImageRef(std::span<uint8_t> data, size_t width, size_t height) :
				m_width(width),
				m_height(height),
				m_data(data) {}
	};

	template<PixelFormat T> class Image {
			size_t m_width;
			size_t m_height;
			std::unique_ptr<uint8_t[]> m_data;

		public:
			explicit Image(size_t width, size_t height, std::span<uint8_t> data) :
				m_width(width),
				m_height(height),
				m_data(new uint8_t[data.size()]) {
				VERIFY(width * height * bytes_per_pixel(T) == data.size());
				std::memcpy(m_data.get(), data.data(), data.size_bytes());
			}

			explicit Image(size_t width, size_t height, uint8_t* data) :
				m_width(width),
				m_height(height),
				m_data(data) {}

			size_t width() {
				return m_width;
			}

			size_t height() {
				return m_height;
			}

			size_t num_pixels() {
				return m_width * m_height;
			}

			size_t num_bytes() {
				return num_pixels() * bytes_per_pixel(T);
			}

			std::span<uint8_t> data() {
				return std::span(m_data.get(), num_bytes());
			}

			operator ImageRef<T>() {
				return ImageRef<T>(data(), width(), height());
			}

			ImageRef<T> ref() {
				return *this;
			}
	};

	Image<PixelFormat::RGBA8888> from_file(std::filesystem::path path);
} // namespace graphics
