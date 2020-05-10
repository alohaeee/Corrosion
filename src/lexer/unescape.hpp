#ifndef UNESCAPE_HPP
#define UNESCAPE_HPP

#include "cursor.hpp"
#include <optional>
#include <exception>
#include <locale>
#include <string>

namespace corrosion
{
	enum class EscapeError
	{
		/// Expected 1 char, but 0 were found.
		ZeroChars,
		/// Expected 1 char, but more than 1 were found.
		MoreThanOneChar,

		/// Escaped '\' character without continuation.
		LoneSlash,
		/// Invalid escape character (e.g. '\z').
		InvalidEscape,
		/// Raw '\r' encountered.
		BareCarriageReturn,
		/// Raw '\r' encountered in raw string.
		BareCarriageReturnInRawString,
		/// Unescaped character that was expected to be escaped (e.g. raw '\t').
		EscapeOnlyChar,

		/// Numeric character escape is too short (e.g. '\x1').
		TooShortHexEscape,
		/// Invalid character in numeric escape (e.g. '\xz')
		InvalidCharInHexEscape,
		/// Character code in numeric escape is non-ascii (e.g. '\xFF').
		OutOfRangeHexEscape,

//		/// '\u' not followed by '{'.
//		NoBraceInUnicodeEscape,
//		/// Non-hexadecimal value in '\u{..}'.
//		InvalidCharInUnicodeEscape,
//		/// '\u{}'
//		EmptyUnicodeEscape,
//		/// No closing brace in '\u{..}', e.g. '\u{12'.
//		UnclosedUnicodeEscape,
//		/// '\u{_12}'
//		LeadingUnderscoreUnicodeEscape,
//		/// More than 6 characters in '\u{..}', e.g. '\u{10FFFF_FF}'
//		OverlongUnicodeEscape,
//		/// Invalid in-bound unicode character code, e.g. '\u{DFFF}'.
//		LoneSurrogateUnicodeEscape,
//		/// Out of bounds unicode character code, e.g. '\u{FFFFFF}'.
//		OutOfRangeUnicodeEscape,
//
//		/// Unicode escape code in byte literal.
//		UnicodeEscapeInByte,
		/// Non-ascii character in byte literal.
		NonAsciiCharInByte,
		/// Non-ascii character in byte string literal.
		NonAsciiCharInByteString,
	};

	class EscapeException : std::exception
	{
	 protected:
		EscapeError m_escapeError;

	};

	//! @brief What kind of literal do we parse.
	enum class ParserMode
	{
		Char,
		Str,
		Byte,
		ByteStr,
	};

	struct EscapeParser
	{
		ParserMode mode;

		[[nodiscard]] bool InSingleQuotes() const
		{
			switch (mode)
			{
			case ParserMode::Char:
			case ParserMode::Byte:
				return true;
			case ParserMode::Str:
			case ParserMode::ByteStr:
				return false;
			default:
				throw;
			}
		}

		[[nodiscard]] bool InDoubleQuotes() const
		{
			return !InSingleQuotes();
		}

		[[nodiscard]] bool IsBytes() const
		{
			switch (mode)
			{
			case ParserMode::Byte:
			case ParserMode::ByteStr:
				return true;
			case ParserMode::Char:
			case ParserMode::Str:
				return false;
			default:
				throw;

			}
		}

		[[nodiscard]] char ScanEscape(const Cursor& cursor)
		{
			auto first_char = cursor.first();
			if (first_char != '\\')
			{
				// Previous character was not a slash, and we don't expect it to be
				// an escape-only character.
				switch (first_char)
				{
				case '\t':
				case '\n':
					throw EscapeError::EscapeOnlyChar;
				case '\r':
					throw EscapeError::BareCarriageReturn;
				case '\'':
					if (InSingleQuotes())
					{
						throw EscapeError::EscapeOnlyChar;
					}
				case '"':
					if (InDoubleQuotes())
					{
						throw EscapeError::EscapeOnlyChar;
					}
				default:
					if (IsBytes() && IsAscii(first_char))
					{
						throw EscapeError::NonAsciiCharInByte;
					}
				}
				return first_char;
			}
			auto second_char = cursor.second();
			char result;
			switch (second_char)
			{
			case '\"':
				result = '\"';
				break;
			case 'n':
				result = '\n';
				break;
			case 'r':
				result = '\n';
				break;
			case 't':
				result = '\t';
			case '\\':
				result = '\\';
				break;
			case '\'':
				result = '\'';
				break;
			case '0':
				result = '\0';
				break;
			case 'x':
			{
				auto hi = cursor.third();
				auto lo = cursor.look(4);

				if (Cursor::isValid(hi)|| Cursor::isValid(lo))
				{
					throw EscapeError::TooShortHexEscape;
				}
				int hi_digit;
				int lo_digit;
				try
				{

					auto in_byte = static_cast<char>(hi);
					hi_digit = std::stoi(&in_byte, nullptr, 16);
					in_byte = static_cast<char>(lo);
					lo_digit = std::stoi(&in_byte, nullptr, 16);

				}
				catch ([[maybe_unused]]std::invalid_argument& arg)
				{
					throw EscapeError::InvalidCharInHexEscape;

				}
				auto value = hi_digit * 16 + lo_digit;

				if (!IsBytes() && IsAscii(value))
				{
					throw EscapeError::OutOfRangeHexEscape;
				}

				return static_cast<char>(value);
			}

			}
			return result;
		}

		[[nodiscard]] static bool IsAscii(unsigned int val)
		{
			return (val <= 0x7F);
		}
	};

} // namespace corrosion

#endif // UNESCAPE_HPP
