#pragma once

#include <iostream>
#include <istream>
#include <string>

#include "assets/Asset.h"

namespace assets::types {
	class TextAsset: public Asset {
			std::string m_text;

		public:
			explicit TextAsset(std::istream* in) : m_text(std::istreambuf_iterator<char>(*in), {}) {}

			std::string const& text() const {
				return m_text;
			}
	};
}; // namespace assets::types