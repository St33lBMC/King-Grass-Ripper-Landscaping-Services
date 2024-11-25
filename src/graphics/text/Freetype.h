#pragma once
#include <freetype2/ft2build.h>

#include <cstdint>
#include <string_view>

#include "graphics/image/Image.h"
#include "utils/Verify.h"
#include FT_FREETYPE_H
#include FT_MODULE_H
#include FT_BITMAP_H

namespace graphics::text::freetype {

	class Library {
			FT_Library m_library;

		public:
			Library() {
				auto error = FT_Init_FreeType(&m_library);
				if (error) {
					PANIC("freetype init failed: {}", FT_Error_String(error));
				}
			}

			FT_Library raw() {
				return m_library;
			}

			~Library() {
				FT_Done_FreeType(m_library);
			}
	};

	class FontFace {
			Library& m_library;
			FT_Face m_face;

		public:
			using CharIndex = uint32_t;

			FontFace(Library& library, const char* file_path_name, long face_index) : m_library(library) {
				auto error = FT_New_Face(library.raw(), file_path_name, face_index, &m_face);
				if (error) {
					PANIC("freetype font face creation failed: {}", FT_Error_String(error));
				}
			}

			void set_char_size(
				long char_width,
				long char_height,
				unsigned int horizontal_resolution,
				unsigned int vertical_resolution
			) {
				auto error =
					FT_Set_Char_Size(m_face, char_width, char_height, horizontal_resolution, vertical_resolution);
				if (error) {
					PANIC("freetype char size set failed: {}", FT_Error_String(error));
				}
			}

			void set_pixel_size(unsigned int pixel_width, unsigned int pixel_height) {
				auto error = FT_Set_Pixel_Sizes(m_face, pixel_width, pixel_height);
				if (error) {
					PANIC("freetype char pixel size set failed: {}", FT_Error_String(error));
				}
			}

			CharIndex char_index(unsigned long char_code) {
				return FT_Get_Char_Index(m_face, char_code);
			}

			void load_glyph(CharIndex char_index, uint32_t load_flags) {
				auto error = FT_Load_Glyph(m_face, char_index, load_flags);
				if (error) {
					PANIC("freetype glyph load failed: {}", FT_Error_String(error));
				}
			}

			void render_glyph(FT_Render_Mode render_mode = FT_RENDER_MODE_NORMAL) {
				auto error = FT_Render_Glyph(m_face->glyph, render_mode);
				if (error) {
					PANIC("freetype glyph render failed: {}", FT_Error_String(error));
				}
			}

			graphics::ImageRef<PixelFormat::Grayscale8> bitmap() {
				VERIFY(m_face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY, "Not grayscale font bitmap");

				auto& bitmap = m_face->glyph->bitmap;
				fmt::print("Width {} rows {} pitch {}\n", bitmap.width, bitmap.rows, bitmap.pitch);
				return ImageRef<PixelFormat::Grayscale8>(
					std::span(bitmap.buffer, bitmap.width * bitmap.rows),
					bitmap.width,
					bitmap.rows
				);
			}

			FT_Face raw() {
				return m_face;
			}

			FontFace(FontFace& other) : m_library(other.m_library), m_face(other.m_face) {
				FT_Reference_Face(m_face);
			}

			~FontFace() {
				FT_Done_Face(m_face);
			}
	};

}; // namespace graphics::text::freetype
