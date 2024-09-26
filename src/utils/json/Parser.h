#pragma once
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <istream>
#include <memory>
#include <optional>
#include <ostream>
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

	class ParseException: public std::exception {
		private:
			std::string m_information;

		public:
			ParseException(std::string info, size_t location) : m_information(info + " " + std::to_string(location)) {}

			const char* what() const noexcept override {
				return m_information.c_str();
			}
	};

	struct Tokenized {
			std::vector<std::string> m_string_table;
			std::vector<Token> m_tokens;
			size_t m_position;

			void assert_text(std::istream& stream, std::string_view t);

			Token next() {
				if (m_position < m_tokens.size()) {
					return m_tokens[m_position++];
				} else {
					throw ParseException("EOF", m_position);
				}
			};

			Token peek() {
				if (m_position < m_tokens.size()) {
					return m_tokens[m_position];
				} else {
					throw ParseException("EOF", m_position);
				}
			}

			Tokenized(std::istream& stream);
	};

	class JSONValue {
		public:
			static std::unique_ptr<JSONValue> parse(Tokenized& tokens);
			virtual void parse_from(Tokenized& tokens) = 0;
			virtual void display(std::ostream& out) = 0;

			virtual ~JSONValue() {}
	};

	struct JSONArray: public JSONValue {
		std::vector<std::unique_ptr<JSONValue>> m_values;
	
		
	};

	struct JSONPrimitive: public JSONValue {
			enum { Null, Boolean, Number } m_tag;

			union {
					double number;
					bool boolean;
			} m_value;

			virtual void display(std::ostream& out) override {
				switch (m_tag) {
					case Null:
						out << "null";
						break;
					case Boolean:
						out << this->m_value.boolean;
						break;
					case Number:
						out << this->m_value.number;
						break;
				}
			}

			virtual void parse_from(Tokenized& tokens) override {
				switch (tokens.peek().m_token_type) {
					case TokenType::Null:
						tokens.next();
						this->m_tag = Null;
						break;
					case TokenType::Number:
						this->m_tag = Number;
						this->m_value.number = tokens.next().m_token_value.number;
						break;
					case TokenType::Boolean:
						this->m_tag = Boolean;
						this->m_value.boolean = tokens.next().m_token_value.boolean;
						break;
					default:
						throw ParseException("Not primitive", tokens.m_position);
						break;
				}
			}
	};

} // namespace utils::json