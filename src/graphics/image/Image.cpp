#include "graphics/image/Image.h"

#include <png.h>

#include "utils/Verify.h"

namespace graphics {

	RGBA8888Image RGBA8888Image::from_file(std::filesystem::path path) {
		png_image image;
		memset(&image, 0, sizeof(image));
		image.version = PNG_IMAGE_VERSION;

		if (png_image_begin_read_from_file(&image, path.c_str()) != 0) {
			png_bytep buffer;
			image.format = PNG_FORMAT_RGBA;
			buffer = new uint8_t[PNG_IMAGE_SIZE(image)];
			if (buffer != NULL && png_image_finish_read(&image, nullptr, buffer, 0 /*row_stride*/, nullptr) != 0) {
				// FIXME: alignment OK here or not?
				auto i = RGBA8888Image(image.width, image.height, reinterpret_cast<uint32_t*>(buffer));
				return i;
			}
		}
		VERIFY_NOT_REACHED();
	}
} // namespace graphics