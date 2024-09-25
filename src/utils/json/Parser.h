#pragma once
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <istream>
#include <string>
#include <string_view>
#include <vector>

namespace utils::json {
	enum class TokenType {
		LCurlyBracket,
		RCurlyBracket,
		LBracket,
		RBracket,
		Colon,
		Comma,
		Null,
		Boolean,
		Number,
		String,
	};

	struct Token {
			TokenType m_token_type;

			union {
					double number;
					std::size_t string;
					bool boolean;
			} m_token_value;

			inline constexpr static Token number(double number) {
				Token s;
				s.m_token_type = TokenType::Number;
				s.m_token_value.number = number;
				return s;
			}

			inline constexpr static Token string(size_t string) {
				Token s;
				s.m_token_type = TokenType::String;
				s.m_token_value.string = string;
				return s;
			}

			inline constexpr static Token boolean(bool boolean) {
				Token s;
				s.m_token_type = TokenType::Boolean;
				s.m_token_value.boolean = boolean;
				return s;
			}

			inline constexpr static Token null() {
				return valueless(TokenType::Null);
			}

			inline constexpr static Token valueless(TokenType type) {
				Token s;
				s.m_token_type = type;
				return s;
			}
	};

	class TokenizerException: public std::exception {
		private:
			std::string m_information;

		public:
			TokenizerException(std::string info, size_t location) :
				m_information(info + " " + std::to_string(location)) {}

			const char* what() const noexcept override {
				return m_information.c_str();
			}
	};

	struct Tokenized {
			std::vector<std::string> m_string_table;
			std::vector<Token> m_tokens;

			void assert_text(std::istream& stream, std::string_view t);

			Tokenized(std::istream& stream);
	};
} // namespace utils::json