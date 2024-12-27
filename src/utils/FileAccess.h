#pragma once
#include <cstring>
#include <fstream>
#include <string>

#include "utils/Verify.h"

namespace utils {
	inline std::string get_file_contents(const char* filename) {
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in) {
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return (contents);
		}
		PANIC("read of {} failed: {}", filename, std::strerror(errno));
	}
} // namespace utils
