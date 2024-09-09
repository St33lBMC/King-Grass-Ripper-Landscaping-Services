#include "utils/json/Parser.h"

namespace utils::json {

	void Tokenized::assert_text(std::istream& stream, std::string_view t) {
		for (char c : t) {
			if (stream.get() != c) {
				throw TokenizerException(stream.tellg());
			}
		}
	}

	Tokenized::Tokenized(std::istream& stream) {
		while (!stream.eof() && stream.peek() != EOF) {
			auto peek = stream.peek();
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
				default:
					if (std::isspace(peek)) {
						stream.get();
						continue;
					}
					throw TokenizerException(stream.tellg());
					break;
			}
		}
	}
} // namespace utils::json
