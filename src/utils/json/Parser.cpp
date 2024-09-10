#include "utils/json/Parser.h"

#include <cctype>
#include <cstddef>
#include <cstdio>
#include <string>

namespace utils::json {

	void Tokenized::assert_text(std::istream& stream, std::string_view t) {
		for (char c : t) {
			if (stream.get() != c) {
				throw TokenizerException("text assert failure", stream.tellg());
			}
		}
	}

	std::string read_until(std::istream& stream, char target) {
		std::string s;
		while (!stream.eof()) {
			auto x = stream.get();
			if (x == target) {
				return s;
			}
			s += x;
		}
		throw TokenizerException("did not find end target", stream.tellg());
	}

	Tokenized::Tokenized(std::istream& stream) {
		while (!stream.eof() && stream.peek() != EOF) {
			char peek = stream.peek();
			std::cout << peek << std::endl;
			switch (peek) {
				case '{':
					stream.get();
					m_tokens.push_back(Token::valueless(TokenType::LCurlyBracket));
					break;
				case '}':
					stream.get();
					m_tokens.push_back(Token::valueless(TokenType::RCurlyBracket));
					break;
				case '[':
					stream.get();
					m_tokens.push_back(Token::valueless(TokenType::LBracket));
					break;
				case ']':
					stream.get();
					m_tokens.push_back(Token::valueless(TokenType::RBracket));
					break;
				case ':':
					stream.get();
					m_tokens.push_back(Token::valueless(TokenType::Colon));
					break;
				case ',':
					stream.get();
					m_tokens.push_back(Token::valueless(TokenType::Comma));
					break;
				case 't':
					stream.get();
					assert_text(stream, "rue");
					m_tokens.push_back(Token::boolean(true));
					break;
				case 'f':
					stream.get();
					assert_text(stream, "alse");
					m_tokens.push_back(Token::boolean(false));
					break;
				case 'n':
					stream.get();
					assert_text(stream, "ull");
					m_tokens.push_back(Token::null());
					break;
				case '"': {
					stream.get();
					std::string t = read_until(stream, '"');
					size_t index = m_string_table.size();
					m_string_table.push_back(t);
					m_tokens.push_back(Token::string(index));
					break;
				}
				default:
					if (std::isspace(peek)) {
						stream.get();
						continue;
					} else if (std::isdigit(peek)) {
						stream.get();
						std::string build = std::to_string(peek);
						bool dotted = false;
						while (!stream.eof() && stream.peek() != EOF) {
							char peek2 = stream.peek();
							if (std::isdigit(peek2)) {
								stream.get();
								build.append(&peek2, 1);
							} else if (peek2 == '.' && !dotted) {
								stream.get();
								build.append(&peek2, 1);
								dotted = true;
							} else {
								break;
							}
						}
						m_tokens.push_back(Token::number(std::stod(build)));
					} else {
						throw TokenizerException("unknown character", stream.tellg());
					}
					break;
			}
		}
	}
} // namespace utils::json
