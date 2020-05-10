#ifndef CORROSION_LEXER_HPP
#define CORROSION_LEXER_HPP

#include <cassert>
#include <variant>
#include <string>
#include <type_traits>
#include <iomanip>

#include "cursor.hpp"
#include "unescape.hpp"

namespace corrosion
{
	enum class TokenKind
	{
		// Multi-char tokens:
		/// "// comment"
		LineComment,
		/// "/* block comment */"
		/// Block comments can be recursive, so the sequence like "/* /* */"
		/// will not be considered terminated and will result in a parsing error.
		BlockComment,
		/// Any whitespace characters sequence.
		Whitespace,
		/// "ident" or "continue"
		/// At this step keywords are also considered identifiers.
		Identifier,
		/// "r#ident"
		RawIdentifier,
		/// "12_u8", "1.0e-40", "b"123"". See `LiteralKind` for more details.
		Literal,

		/// "'a"
		Lifetime,

		// One-char tokens:
		/// ";"
		Semi,
		/// ","
		Comma,
		/// "."
		Dot,
		/// "("
		OpenParen,
		/// ")"
		CloseParen,
		/// "{"
		OpenBrace,
		/// "}"
		CloseBrace,
		/// "["
		OpenBracket,
		/// "]"
		CloseBracket,
		/// "@"
		At,
		/// "#"
		Pound,
		/// "~"
		Tilde,
		/// "?"
		Question,
		/// ":"
		Colon,
		/// "$"
		Dollar,
		/// "="
		Eq,
		/// "!"
		Not,
		/// "<"
		Lt,
		/// ">"
		Gt,
		/// "-"
		Minus,
		/// "&"
		And,
		/// "|"
		Or,
		/// "+"
		Plus,
		/// "*"
		Star,
		/// "/"
		Slash,
		/// "^"
		Caret,
		/// "%"
		Percent,

		/// Unknown token, not expected by the lexer, e.g. "№"
		Unknown
	};
	enum class Base
	{
		/// Literal starts with "0b".
		Binary,
		/// Literal starts with "0o".
		Octal,
		/// Literal starts with "0x".
		Hexadecimal,
		/// Literal doesn't contain a prefix.
		Decimal
	};

	namespace literal_types
	{
		/// "12_u8", "0o100", "0b120i99"
		struct Int
		{
			Base base;
			bool emptyInt;
			std::size_t suffixStart;
		};
		/// "12.34f32", "0b100.100"
		struct Float
		{
			Base base;
			bool emptyExponent;
			std::size_t suffixStart;
		};
		/// "'a'", "'\\'", "'''", "';"
		struct Char
		{
			bool terminated;
			std::size_t suffixStart;
		};
		/// ""abc"", ""abc"
		struct Str
		{
			bool terminated;
			std::size_t suffixStart;
		};
		/// Sign that literal kind variant is empty.
		struct Empty
		{
		};
		//		/// "b'a'", "b'\\'", "b'''", "b';"
//		struct Byte
//		{
//			bool terminated;
//		} byteKind;
		/// "b"abc"", "b"abc"
//		struct ByteStr
//		{
//			bool terminated;
//		} byteStr_kind;
	}
	namespace token_data
	{
		struct BlockComment
		{
			bool terminated;
		};
		struct Lifetime
		{
			bool startWithNumber;
		};
		using LiteralKind = std::variant<literal_types::Int,
										 literal_types::Float,
										 literal_types::Char,
										 literal_types::Str,
										 literal_types::Empty>;
		struct Empty
		{

		};
	}

	using TokenData = std::variant<token_data::LiteralKind, token_data::BlockComment,
								   token_data::Lifetime, token_data::Empty>;

	// helper type for the visitor
	template<class T>
	struct always_false : std::false_type
	{
	};

	struct Token
	{
		TokenKind kind = TokenKind::Unknown;
		std::size_t consumed = 0;
		TokenData data;
	};

	struct Alphabet
	{
		inline static bool isWhitespace(char c)
		{
			switch (c)
			{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				return true;
			default:
				return false;
			}
		}
		inline static bool isIdenStart(char c)
		{
			return isAlpha(c) || c == '_';
		}
		inline static bool isIdenContinue(char c)
		{
			return isAlpha(c) || c == '_' || isDecDigit(c);
		}
		inline static bool isDecDigit(char c)
		{
			return c >= '0' && c <= '9';
		}
		inline static bool isHexDigit(char c)
		{
			return isDecDigit(c) || (c >= 'a' && c <= 'f')
				|| (c >= 'A' && c <= 'B');
		}
		inline static bool isOctDigit(char c)
		{
			return c >= '0' && c <= '7';
		}
		inline static bool isBinDigit(char c)
		{
			return c >= '0' && c <= '1';
		}
		inline static bool isAlpha(char c)
		{
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		}

	};
	struct Tokenizer : public Cursor
	{
	 public:
		static std::string tokenKindPrintable(TokenKind kind)
		{
			switch (kind)
			{
			case TokenKind::Unknown:
				return "Unknown";
			case TokenKind::Identifier:
				return "Identifier";
			case TokenKind::Whitespace:
				return "Whitespace";
			case TokenKind::Lifetime:
				return "Lifetime";
			case TokenKind::Dot:
				return ".";
			case TokenKind::Comma:
				return ",";
			case TokenKind::Semi:
				return ";";
			case TokenKind::Colon:
				return ":";
			case TokenKind::BlockComment:
				return "BlockComment";
			case TokenKind::LineComment:
				return "LineComment";
			case TokenKind::And:
				return "&";
			case TokenKind::Or:
				return "|";
			case TokenKind::Caret:
				return "^";
			case TokenKind::At:
				return "@";
			case TokenKind::Pound:
				return "#";
			case TokenKind::Tilde:
				return "~";
			case TokenKind::Dollar:
				return "$";
			case TokenKind::Eq:
				return "=";
			case TokenKind::Not:
				return "!";
			case TokenKind::Lt:
				return "<";
			case TokenKind::Gt:
				return ">";
			case TokenKind::Minus:
				return "-";
			case TokenKind::Plus:
				return "+";
			case TokenKind::Star:
				return "*";
			case TokenKind::Slash:
				return "/";
			case TokenKind::Percent:
				return "%";
			case TokenKind::Question:
				return "?";
			case TokenKind::OpenParen:
				return "(";
			case TokenKind::CloseParen:
				return ")";
			case TokenKind::OpenBrace:
				return "{";
			case TokenKind::CloseBrace:
				return "}";
			case TokenKind::OpenBracket:
				return "[";
			case TokenKind::CloseBracket:
				return "]";
			case TokenKind::Literal:
				return "Literal";
			default:
				return "";
			}
		}
		static std::string literalBasePrintable(Base base)
		{
			switch (base)
			{
			case Base::Binary:
				return "Binary";
			case Base::Decimal:
				return "Decimal";
			case Base::Hexadecimal:
				return "Hexadecimal";
			case Base::Octal:
				return "Octal";
			default:
				return "";
			}
		}
		static std::string literalKindPrintable(const token_data::LiteralKind& kind)
		{
			return std::visit([](auto&& arg) -> std::string
			{
			  using T = std::decay_t<decltype(arg)>;
			  if constexpr (std::is_same_v<T, literal_types::Int>)
			  {
				  return std::string("Literal kind: Integer; ") + std::string("Base: ") + literalBasePrintable(arg.base);
			  }
			  else if constexpr(std::is_same_v<T, literal_types::Float>)
			  {
				  return std::string("Literal kind: Float; ") + std::string("Base: ") + literalBasePrintable(arg.base);
			  }
			  else if constexpr(std::is_same_v<T, literal_types::Char>)
			  {
				  return "Literal kind: Char";
			  }
			  else if constexpr (std::is_same_v<T, literal_types::Str>)
			  {
				  return "Literal kind: String";
			  }
			  else if constexpr (std::is_same_v<T, literal_types::Empty>)
			  {
				  return "";
			  }
			  else
			  {
				  static_assert(always_false<T>::value, "non-exhaustive visitor for LiteralKind!");
			  }
			}, kind);

		}
		static std::string tokenDataPrintable(const TokenData data)
		{
			return std::visit([](auto&& arg) -> std::string
			{
			  using T = std::decay_t<decltype(arg)>;
			  if constexpr (std::is_same_v<T, token_data::LiteralKind>)
			  {
				  return std::string("TokenData: \"") + literalKindPrintable(arg) + std::string("\"");
			  }
			  else if constexpr(std::is_same_v<T, token_data::BlockComment>)
			  {
				  return std::string("TokenData: \"Block Comment is ") + std::string((arg.terminated?"terminated\"" : "!not terminated!\"")) ;
			  }
			  else if constexpr(std::is_same_v<T, token_data::Lifetime>)
			  {
				  return std::string("TokenData: \"Lifetime ") + std::string((arg.startWithNumber?"!start with number!\"":"dont start with number\""));
			  }
			  else if constexpr (std::is_same_v<T, token_data::Empty>)
			  {
				  return "";
			  }
			  else
			  {
				  static_assert(always_false<T>::value, "non-exhaustive visitor for LiteralKind!");
			  }
			}, data);
		}
		Tokenizer() : Cursor()
		{
		}

		Tokenizer(std::string_view text) : Cursor(text)
		{
		}

		Token advanceToken()
		{
			auto init_len = this->consumed();
			TokenKind token_kind = TokenKind::Unknown;
			TokenData token_data = token_data::Empty();
			auto first_char = this->shift();

			switch (first_char)
			{
			case '/':
			{
				switch (this->first())
				{
				case '/':
					token_kind = this->lineComment();
					break;
				case '*':
				{
					auto&&[kind, data] = this->blockComment();
					token_kind = kind;
					token_data = data;
					break;
				}

				default:
					token_kind = TokenKind::Slash;
				}
				break;
			}
			case 'r':
			{
				auto first = this->first();
				auto second = this->second();
				if (first == '#' && Alphabet::isIdenStart(second))
				{
					token_kind = this->rawIdentifier();
					break;
				}
				else if (first == '#' || first == '"')
				{

				}
				else
				{
					token_kind = identifier();
				}
				break;
			}
			default:
				if (Alphabet::isIdenStart(first_char))
				{
					token_kind = identifier();
				}
				else if (Alphabet::isWhitespace(first_char))
				{
					token_kind = whitespace();
				}
				else if (Alphabet::isDecDigit(first_char))
				{
					token_data = token_data::LiteralKind {this->number()};
					token_kind = TokenKind::Literal;
				}
				break;

				// One-symbol tokens.
			case ';':
				token_kind = TokenKind::Semi;
				break;
			case ',':
				token_kind = TokenKind::Comma;
				break;
			case '.':
				token_kind = TokenKind::Dot;
				break;
			case '(':
				token_kind = TokenKind::OpenParen;
				break;
			case ')':
				token_kind = TokenKind::CloseParen;
				break;
			case '{':
				token_kind = TokenKind::OpenBrace;
				break;
			case '}':
				token_kind = TokenKind::CloseBrace;
				break;
			case '[':
				token_kind = TokenKind::OpenBracket;
				break;
			case ']':
				token_kind = TokenKind::CloseBracket;
				break;
			case '@':
				token_kind = TokenKind::At;
				break;
			case '#':
				token_kind = TokenKind::Pound;
				break;
			case '~':
				token_kind = TokenKind::Tilde;
				break;
			case '?':
				token_kind = TokenKind::Question;
				break;
			case ':':
				token_kind = TokenKind::Colon;
				break;
			case '$':
				token_kind = TokenKind::Dollar;
				break;
			case '=':
				token_kind = TokenKind::Eq;
				break;
			case '!':
				token_kind = TokenKind::Not;
				break;
			case '<':
				token_kind = TokenKind::Lt;
				break;
			case '>':
				token_kind = TokenKind::Gt;
				break;
			case '-':
				token_kind = TokenKind::Minus;
				break;
			case '&':
				token_kind = TokenKind::And;
				break;
			case '|':
				token_kind = TokenKind::Or;
				break;
			case '+':
				token_kind = TokenKind::Plus;
				break;
			case '*':
				token_kind = TokenKind::Star;
				break;
			case '^':
				token_kind = TokenKind::Caret;
				break;
			case '%':
				token_kind = TokenKind::Percent;
				break;

			case '\'':
			{
				auto &&[kind, data] = lifetimeOrChar();
				token_kind = kind;
				token_data = data;
				break;
			}
			case '"':
				auto terminated = this->doubleQuotedString();
				auto suffix_start = this->consumed();
				if (terminated)
				{
					this->eatLiteralSuffix();
				}
				token_data = token_data::LiteralKind{literal_types::Str{ terminated, suffix_start }};
				token_kind = TokenKind::Literal;
				break;

			}
			return Token{ token_kind, this->consumed() - init_len, token_data };
		}
	 protected:
		TokenKind lineComment()
		{
			assert(this->prev() == '/' && this->first() == '/');
			this->shift();
			eatWhile([](char c)
			{ return c != '\n'; });

			return TokenKind::LineComment;
		}
		std::pair<TokenKind, TokenData> blockComment()
		{
			assert(this->prev() == '/' && this->first() == '*');
			this->shift();
			auto c = this->shift();
			std::size_t depth = 1u;

			while (this->isValid(c))
			{
				if (c == '/')
				{
					if (this->first() == '*')
					{
						this->shift();
						depth++;
					}
				}
				else if (c == '*')
				{
					if (this->first() == '/')
					{
						this->shift();
						depth --;
						if (depth == 0)
						{
							// This block comment is closed, so for a construction like "/* */ */"
							// there will be a successfully parsed block comment "/* */"
							// and " */" will be processed separately.
							break;
						}
					}
				}
				c = this->shift();
			}
			return { TokenKind::BlockComment, token_data::BlockComment{ depth == 0 }};
		}
		TokenKind
		whitespace()
		{
			assert(Alphabet::isWhitespace(this->prev()));
			this->eatWhile(Alphabet::isWhitespace);

			return TokenKind::Whitespace;
		}
		TokenKind identifier()
		{
			assert(Alphabet::isIdenStart(this->prev()));
			this->eatWhile(Alphabet::isIdenContinue);

			return TokenKind::Identifier;
		}

		TokenKind rawIdentifier()
		{
			assert(this->prev() == 'r' && this->first() == '#' && Alphabet::isIdenStart(this->second()));
			// eat '#' symbol.
			this->shift();
			this->identifier();

			return TokenKind::RawIdentifier;
		}

		token_data::LiteralKind number()
		{
			assert(this->prev() >= '0' && this->prev() <= '9');
			Base base = Base::Decimal;
			token_data::LiteralKind kind{};

			if (this->prev() == '0')
			{
				// Attempt to parse encoding base.
				bool has_digits = false;
				auto first = this->first();
				if (first == 'b')
				{
					base = Base::Binary;
					this->shift();
					has_digits = this->eatDecimalDigits();
				}
				else if (first == 'o')
				{
					base = Base::Octal;
					this->shift();
					has_digits = this->eatDecimalDigits();
				}
				else if (first == 'x')
				{
					base = Base::Hexadecimal;
					this->shift();
					has_digits = this->eatHexadecimalDigits();
				}
					// Not a base prefix.
				else if (Alphabet::isDecDigit(first) || first == '_' || first == '.' || first == 'e'
					|| first == 'E')
				{
					this->eatDecimalDigits();
					has_digits = true;
				}
					// Just a 0.
				else
				{
					auto suffix_start = this->consumed();
					this->eatLiteralSuffix();
					kind = literal_types::Int{ base, false, suffix_start };
					return kind;
				}
				// Base prefix was provided, but there were no digits
				// after it, e.g. "0x".
				if (!has_digits)
				{
					auto suffix_start = this->consumed();
					this->eatLiteralSuffix();
					kind = literal_types::Int{ base, true };
					return kind;
				}

			}
			else
			{
				// No base prefix, parse number in the usual way.
				this->eatDecimalDigits();
			}
			// Don't be greedy if this is actually an
			// integer literal followed by field/method access or a range pattern
			// (`0..2` and `12.foo()`)
			auto first = this->first();
			if (first == '.')
			{
				if (this->second() != '.' && !Alphabet::isIdenStart(this->second()))
				{
					this->shift();
					bool empty_exponent = false;
					if (Alphabet::isDecDigit(this->first()))
					{
						this->eatDecimalDigits();
						if (this->first() == 'e' || this->first() == 'E')
						{
							this->shift();
							empty_exponent = this->eatFloatExponent();
						}
					}
					auto suffix_start = this->consumed();
					this->eatLiteralSuffix();
					kind = literal_types::Float{ base, empty_exponent };
					return kind;
				}
			}
			else if (first == 'e' || first == 'E')
			{
				this->shift();
				auto empty_exponent = this->eatFloatExponent();

				auto suffix_start = this->consumed();
				this->eatLiteralSuffix();
				kind = literal_types::Float{ base, empty_exponent };
				return kind;
			}

			auto suffix_start = this->consumed();
			this->eatLiteralSuffix();
			kind = literal_types::Int{ base, false };
			return kind;

		}

		std::pair<TokenKind, TokenData> lifetimeOrChar()
		{
			assert(this->prev() == '\'');
			bool can_be_lifetime;
			if (this->second() == '\'')
			{
				// It's surely not a lifetime.
				can_be_lifetime = false;
			}
			else
			{
				// If the first symbol is valid for identifier, it can be a lifetime.
				// Also check if it's a number for a better error reporting (so '0 will
				// be reported as invalid lifetime and not as unterminated char literal).
				can_be_lifetime = Alphabet::isIdenStart(this->first()) || Alphabet::isDecDigit(this->first());
			}
			if (!can_be_lifetime)
			{
				auto terminated = this->singleQuotedString();
				auto suffix_start = this->consumed();

				if (terminated)
				{
					this->eatLiteralSuffix();
				}
				auto kind = literal_types::Char{ terminated, suffix_start };
				return { TokenKind::Literal, token_data::LiteralKind{ kind }};
			}
			// Either a lifetime or a character literal with
			// length greater than 1.
			auto start_with_number = Alphabet::isDecDigit(this->first());

			// Skip the literal contents.
			// First symbol can be a number (which isn't a valid identifier start),
			// so skip it without any checks.
			this->shift();
			this->eatWhile(Alphabet::isIdenContinue);

			// Check if after skipping literal contents we've met a closing
			// single quote (which means that user attempted to create a
			// string with single quotes).
			if (this->first() == '\'')
			{
				this->shift();
				auto kind = literal_types::Char{ true };
				return { TokenKind::Literal, token_data::LiteralKind{ kind }};
			}

			return { TokenKind::Lifetime, token_data::Lifetime{ start_with_number }};
		}

		bool singleQuotedString()
		{
			assert(this->prev() == '\'');
			// Check if it's a one-symbol literal.

			if (this->second() == '\'' && this->first() != '\\')
			{
				this->shift();
				this->shift();
				return true;
			}
			// Literal has more than one symbol.

			// Parse until either quotes are terminated or error is detected.
			for (;;)
			{
				switch (this->first())
				{
					// Quotes are terminated, finish parsing.
				case '\'':
					this->shift();
					return true;
					// Probably beginning of the comment, which we don't want to include
					// to the error report.
				case '/':
					return false;
					// Newline without following '\'' means unclosed quote, stop parsing.
				case '\n':
				{
					if (this->second() != '\'')
					{
						return false;
					}
					break;
				}
					// End of file, stop parsing.
				case EOF:
				{
					if (this->isEof())
					{
						return false;
					}
					break;
				}
					// Escaped slash is considered one character, so bump twice.
				case '\\':
					this->shift();
					this->shift();
					break;
				default:
					this->shift();
				}
			}
		}

		/// Eats double-quoted string and returns true
		/// if string is terminated.
		bool doubleQuotedString()
		{
			assert(this->prev() == '"');
			while (this->isValid(this->shift()))
			{

				switch (this->prev())
				{
				case '"':
					return true;
				case '\\':
				{
					if (this->first() == '\\' || this->first() == '"')
					{
						// Bump again to skip escaped character
						this->shift();
					}
					break;
				}
				}
			}
			return false;
		}

		bool rawDoubleQuotedString()
		{

		}

		bool eatDecimalDigits()
		{
			bool has_digits = false;
			for (;;)
			{
				auto first = this->first();
				if (first == '_')
				{
					this->shift();
				}
				else if (Alphabet::isDecDigit(first))
				{
					has_digits = true;
					this->shift();
				}
				else
				{
					break;
				}
			}
			return has_digits;
		}

		bool eatHexadecimalDigits()
		{
			bool has_digits = false;
			for (;;)
			{
				auto first = this->first();
				if (first == '_')
				{
					this->shift();
				}
				else if (Alphabet::isHexDigit(first))
				{
					has_digits = true;
					this->shift();
				}
				else
				{
					break;
				}
			}
			return has_digits;
		}

		/// Eats the float exponent. Returns true if at least one digit was met,
		/// and returns false otherwise.
		bool eatFloatExponent()
		{
			assert(this->prev() == 'e' || this->prev() == 'E');
			if (this->first() == '-' || this->first() == '+')
			{
				this->shift();
			}
			return eatDecimalDigits();
		}

		// Eats the suffix of the literal, e.g. "_u8".
		void eatLiteralSuffix()
		{
			this->eatIdentifier();
		}

		void eatIdentifier()
		{
			if (!Alphabet::isIdenStart(this->first()))
			{
				return;
			}
			this->shift();
			this->eatWhile(Alphabet::isIdenContinue);
		}

		template<typename Predicate>
		std::size_t eatWhile(Predicate predicate)
		{
			std::size_t eaten = 0u;
			while (predicate(this->first()) && !this->isEof())
			{
				eaten++;
				this->shift();
			}
			return eaten;
		}
	};
}

#endif //CORROSION_LEXER_HPP
