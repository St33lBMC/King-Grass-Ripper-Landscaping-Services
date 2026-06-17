#include "graphics/image/Image.h"

#include "ApprovalTests.hpp"
#include "catch2/catch_test_macros.hpp"

namespace graphics::image {
	template<graphics::image::PixelFormat T>
	std::ostream& operator<<(std::ostream& os, const graphics::image::ImageRef<T>& img) {
		os << img.width() << " " << img.height();
		os.write(reinterpret_cast<const char*>(img.data().data()), img.data().size_bytes());
		return os;
	}
}; // namespace graphics::image

TEST_CASE("image load", "[image][approval]") {
	auto image = graphics::image::from_file("../tests/assets/gullible_on_the_ceiling.png");
	ApprovalTests::Approvals::verify(image.ref());
}
