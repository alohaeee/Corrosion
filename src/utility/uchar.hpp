//
// Created by Vladislav on 06.04.2020.
//

#ifndef UTF8STRING_SRC_CHAR_UCHAR_HPP_
#define UTF8STRING_SRC_CHAR_UCHAR_HPP_

#include <iostream>
#include <ctype.h>
#include <cuchar>
#include <vector>
#include <stdexcept>
#include <variant>

#include "bitwise.hpp"
namespace corrosion
{
	class U8Error : public std::invalid_argument
	{
	 public:
		U8Error(std::size_t valid_up_to, std::size_t error_len, std::string_view msg = "")
			: std::invalid_argument(msg.data()),
			  m_valid_up_to(valid_up_to), m_error_len(error_len)
		{
		};
		U8Error(std::string_view msg)
			: std::invalid_argument(msg.data()),
			  m_valid_up_to(-1), m_error_len(-1)
		{
		};
		// Returns the index in the given string up to which valid UTF-8 was verified.
		[[nodiscard]] std::size_t valid_up_to() const noexcept
		{
			return m_valid_up_to;
		}
		// The length provided is that of the invalid byte sequence that starts at the index given by valid_up_to().
		// 0: the end of the input was reached unexpectedly.
		[[nodiscard]] std::size_t error_len() const noexcept
		{
			return m_error_len;
		}
	 protected:
		std::size_t m_valid_up_to;
		std::size_t m_error_len;
	};
	struct U8Core
	{

		// UTF-8 ranges and tags for encoding characters
		enum ByteMasks : uint8_t
		{
			TAG_CONT = 0b10000000u,
			TAG_TWO_B = 0b11000000u,
			TAG_THREE_B = 0b11100000u,
			TAG_FOUR_B = 0b11110000u,
			CONT_VALUE = 0b00111111u
		};

		enum CodeRange : uint32_t
		{
			RANGE_ONE_B = 0x80u,
			RANGE_TWO_B = 0x800u,
			RANGE_THREE_B = 0x10000u,
			RANGE_MAX = 0x10FFFFu
		};
		static inline std::size_t CHAR_WIDTH[256] =
			{
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, // 0x1F
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, // 0x3F
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, // 0x5F
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, // 0x7F
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, // 0x9F
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, // 0xBF
				0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, // 0xDF
				3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 0xEF
				4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xFF
			};
		/**
		 * @brief Lookup into CHAR_WIDTH table and return size of character.
		 * @param first_byte first byte from byte set of UTF-8 character.
		 * @return Amount bytes in UTF-8 character.
		 */
		static inline std::size_t char_len(std::uint8_t first_byte)
		{
			return U8Core::CHAR_WIDTH[first_byte];
		}
		/**
		 * @brief Count and then return actual string length in utf-8. If string was invalid return std::size_t::max
		 * @param view string_view as container of utf-8 string.
		 * @return Characters count/amount in UTF-8 string.
		 */
		static std::size_t str_len(std::string_view view)
		{
			std::size_t len = 0;
			for (std::size_t i = 0; i < view.size(); i++)
			{
				auto c_len = char_len(view[i]);
				if (c_len)
				{
					i += c_len - 1;
					len ++;
				}
				else
				{
					return -1;
				}

			}
			return len;
		}
		/**
		 * @brief Checks whether is string encoded in utf-8 or throw exception(U8Error) if invalid.
		 * @param view string_view as container of utf-8 string.
		 */
		static void is_valid(std::string_view view)
		{
			std::size_t index = 0;
			std::size_t len = view.size();

			while (index < len)
			{
				auto old_offset = index;

				std::uint8_t first = view[index];
				if (first >= 128)
				{
					auto w = CHAR_WIDTH[first];
					// 2-byte encoding is for codepoints  \u{0080} to  \u{07ff}
					//        first  C2 80        last DF BF
					// 3-byte encoding is for codepoints  \u{0800} to  \u{ffff}
					//        first  E0 A0 80     last EF BF BF
					//   excluding surrogates codepoints  \u{d800} to  \u{dfff}
					//               ED A0 80 to       ED BF BF
					// 4-byte encoding is for codepoints \u{1000}0 to \u{10ff}ff
					//        first  F0 90 80 80  last F4 8F BF BF
					//
					// Use the UTF-8 syntax from the RFC
					//
					// https://tools.ietf.org/html/rfc3629
					// UTF8-1      = %x00-7F
					// UTF8-2      = %xC2-DF UTF8-tail
					// UTF8-3      = %xE0 %xA0-BF UTF8-tail / %xE1-EC 2( UTF8-tail ) /
					//               %xED %x80-9F UTF8-tail / %xEE-EF 2( UTF8-tail )
					// UTF8-4      = %xF0 %x90-BF 2( UTF8-tail ) / %xF1-F3 3( UTF8-tail ) /
					//               %xF4 %x80-8F 2( UTF8-tail )
					switch (w)
					{
					case 2:
					{
						index++;

						if (index >= len)
						{
							throw U8Error{ old_offset, 0 };
						}
						if ((static_cast<uint8_t>(view[index]) & ~CONT_VALUE) != TAG_CONT)
						{
							throw U8Error{ old_offset, 1 };
						}
						break;
					}
					case 3:
					{
						index++;

						if (index >= len)
						{
							throw U8Error{ old_offset, 0 };
						}

						std::uint8_t next_char = view[index];

						if (first == 0xE0u && next_char >= 0xA0u && next_char <= 0xBFu)
						{}
						else if (first >= 0xE1u && first <= 0xECu && next_char >= 0x80u && next_char <= 0xBFu)
						{}
						else if (first == 0xEDu && next_char >= 0x80u && next_char <= 0x9Fu)
						{}
						else if (first >= 0xEEu && first <= 0xEFu && next_char >= 0x80u && next_char <= 0xBFu)
						{}
						else
							throw U8Error{ old_offset, 1 };

						index++;

						if (index >= len)
						{
							throw U8Error{ old_offset, 0 };
						}

						if ((static_cast<uint8_t>(view[index]) & ~CONT_VALUE) != TAG_CONT)
						{
							throw U8Error{ old_offset, 2 };;
						}
						break;

					}

					case 4:
					{
						index++;

						if (index >= len)
						{
							throw U8Error{ old_offset, 0 };
						}

						std::uint8_t next_char = view[index];
						if (first == 0xF0 && next_char >= 0x90 && next_char <= 0xBF)
						{}
						else if (first >= 0xF1 && first <= 0xF3 && next_char >= 0x80 && next_char <= 0xBF)
						{}
						else if (first == 0xF4 && next_char >= 0x80 && next_char <= 0xBF)
						{}
						else
							throw U8Error{ old_offset, 1 };

						index++;
						if (index > len)
						{
							throw U8Error{ old_offset, 0 };
						}
						if ((static_cast<uint8_t>(view[index]) & ~CONT_VALUE) != TAG_CONT)
						{
							throw U8Error{ old_offset, 2 };
						}

						index++;
						if (index > len)
						{
							throw U8Error{ old_offset, 0 };
						}
						if ((static_cast<uint8_t>(view[index]) & ~CONT_VALUE) != TAG_CONT)
						{
							throw U8Error{ old_offset, 3 };
						}
						break;
					}
					default:
						throw U8Error{ old_offset, 1 };
					}

				}
				index++;
			}
		}
		static inline bool is_continue_byte(std::uint8_t byte)
		{
			return (byte & ~CONT_VALUE) == TAG_CONT;
		}

	};
	class Chars;
	class UChar
	{
		friend Chars;
	 public:
		/// The highest valid code point a `char` can have.
		///
		/// A [`char`] is a [Unicode Scalar Value], which means that it is a [Code
		/// Point], but only ones within a certain range. `MAX` is the highest valid
		/// code point that's a valid [Unicode Scalar Value].
		///
		/// [`char`]: ../../std/primitive.char.html
		/// [Unicode Scalar Value]: http://www.unicode.org/glossary/#unicode_scalar_value
		/// [Code Point]: http://www.unicode.org/glossary/#code_point
		union CharData
		{
			char bytes[5]{ '\0' };
			uint32_t code;
		};
	 public:

		/**
		 * @brief Create UChar from char.
		 * @param ch - ASCII - encoded character. If ch is not in ASCII throw U8Error.
		 */
		UChar(char ch = '\0')
		{
			if (static_cast<uint8_t>(ch) < U8Core::RANGE_ONE_B)
			{
				m_data.bytes[0] = ch;
			}
			else
			{
				throw U8Error{ "Attempt to initialize UChar with invalid character. Char is not in ASCII-table. Ch: "
								   + std::to_string(int(ch)) };
			}
		}
		/**
		 * @brief Create UChar from UTF-8 string.
		 * @param view string_view as container of utf-8 string.
		 * @param index Index to first byte of the underlying utf-8 character.
		 * @return UChar on success.
		 */
		[[nodiscard]] static UChar from_str(std::string_view view, std::size_t index = 0)
		{
			if (U8Core::is_continue_byte(view.at(index)))
			{
				throw U8Error{
					"Attempt to initialize UChar with UTF-8 string. But first-byte was continued byte. Index: "
						+ std::to_string(index) };
			}

			auto len = U8Core::char_len(view[index]);
			if (!len)
			{
				throw U8Error{
					"Attempt to initialize UChar with UTF-8 string. But first-byte was invalid. Index: "
						+ std::to_string(index) };
			}
			UChar c;
			for (std::size_t i = 0; i < len; i++)
			{
				c.m_data.bytes[i] = view.at(index + i);
			}
			return c;
		}
		/**
		 * @brief Parse string and then create vector of UChars.
		 * @note If you want to just iterate UTF-8 chars use Chars-iterator class.
		 * @param view string_view as container of utf-8 string.
		 * @return Vector of UTF-8 Chars.
		 */
		[[nodiscard]] static std::vector<UChar> from_str_to_vec(std::string_view view)
		{
			std::vector<UChar> vec{};
			vec.reserve(U8Core::str_len(view));
			for(std::size_t i=0; i < view.size(); i++)
			{
				vec.emplace_back(UChar::from_str(view,i));
				i+=vec.back().len_utf8()-1;
			}
			return vec;
		}
		[[nodiscard]] static UChar from_iterator(std::string_view::iterator begin)
		{
			if (U8Core::is_continue_byte(*begin))
			{
				throw U8Error{
					"Attempt to initialize UChar with UTF-8 string. But first-byte was continued byte. First byte: "
						+ std::to_string(int(*begin)) };
			}

			auto len = U8Core::char_len(*begin);
			if (!len)
			{
				throw U8Error{
					"Attempt to initialize UChar with UTF-8 string. But first-byte was invalid. First byte: "
						+ std::to_string(int(*begin)) };
			}
			UChar c;
			for (std::size_t i = 0; i < len; i++)
			{
				c.m_data.bytes[i] = *(begin + i);
			}
			return c;
		}
		[[nodiscard]] static UChar from_codepoint()
		{
			return {};
		}

		[[nodiscard]] std::size_t len_utf8() const noexcept
		{
			return U8Core::char_len(m_data.bytes[0]);
		}

//		/// @brief Returns the number of 16-bit code units this `char` would need if encoded in UTF-16.
//		[[nodiscard]] std::size_t len_utf16() const noexcept
//		{
//			if ((m_data.code & 0xFFFF) == m_data.code)
//			{ return 1; }
//			return 2;
//		}
		/// @brief Returns `true` if this `char` has the `Alphabetic` property.
		[[nodiscard]] bool is_alphabetic() const noexcept
		{
			return std::isalnum(m_data.code);
		}
		/// @brief Returns `true` if this `char` has the `Lowercase` property.
		[[nodiscard]] bool is_lowercase() const noexcept
		{
			return std::islower(m_data.code);
		}
		/// @brief Returns `true` if this `char` has the `Uppercase` property.
		[[nodiscard]] bool is_uppercase() const noexcept
		{
			return std::isupper(m_data.code);
		}
		/// @brief Returns `true` if this `char` has the `White_Space` property.
		[[nodiscard]] bool is_whitespace() const noexcept
		{
			return std::isspace(m_data.code);
		}
		/// @brief Returns `true` if this `char` satisfies either `is_alphabetic()` or `is_numeric()`.
		[[nodiscard]] bool is_alphanumeric() const noexcept
		{
			return is_alphabetic() || is_numeric();
		}

		/// @brief Returns `true` if this `char` has one of the general categories for numbers.
		[[nodiscard]] bool is_numeric() const noexcept
		{
			return std::isdigit(m_data.code);
		}
		/// @brief Returns `true` if this `char` is asccii .
		[[nodiscard]] bool is_ascii() const noexcept
		{
			return m_data.code < U8Core::RANGE_ONE_B;
		}

		/**
		 * @brief Replace byte in Utf-8 char. I hope you know what you are doing.
		 * @tparam byte Valid byte index from 0 to 3.
		 * @param c Replacing char
		 * @return Ref to UChar object.
		 */
		template<std::size_t byte, typename = std::enable_if_t<byte < 4>>
		UChar& replace(char c) noexcept
		{
			m_data.bytes[byte] = c;
			return *this;
		}
		/**
		 * @brief Return underlying data.
		 * @return CharData union.
		 */
		CharData& data()
		{
			return m_data;
		}
		const CharData& data() const
		{
			return m_data;
		}


		bool operator==(char c) const
		{
			return m_data.bytes[0] == c;
		}
		bool operator!=(char c) const
		{
			return !(*this == c);
		}
		bool operator==(UChar uc) const
		{
			return m_data.code == uc.m_data.code;
		}
		bool operator!=(UChar uc) const
		{
			return !(*this == uc);
		}
		friend std::ostream& operator<<(std::ostream& ostream, const UChar& c);

	 protected:
		CharData m_data;
	};
	inline std::ostream& operator<<(std::ostream& ostream, const UChar& c)
	{
		return ostream << c.m_data.bytes;
	}

	/**
	 * @brief Class - iterator for UChar's.
	 */
	class Chars
	{



	 public:
		class iterator
		{
			inline void move()
			{
				m_prev = m_current;
				m_iter += m_current.len_utf8();
				if (m_left)
				{
					m_current = UChar::from_iterator(m_iter);
					m_left--;
				}
				else
				{
					m_current = UChar{};
				}
			}
		 public:
			using iterator_category = std::input_iterator_tag;
			using value_type = UChar;
			using difference_type = std::uint32_t;
			using reference = const UChar&;
			using pointer = const UChar*;

			iterator(std::string_view::iterator iter, std::size_t left) : m_iter{ iter }, m_left(left)
			{
				if(left)
				{
					m_current = UChar::from_iterator(m_iter);
				}
			}

			iterator operator++()
			{
				move();
				return *this;
			}
			iterator operator++(int)
			{
				auto old = *this;
				move();
				return old;
			}
			reference operator*()
			{
				return *operator->();
			}
			pointer operator->()
			{
				return &m_current;
			}
			bool operator==(iterator& other) const
			{
				return m_iter == other.m_iter;
			}
			bool operator!=(iterator& other) const
			{
				return !(m_iter == other.m_iter);
			}
			std::optional<UChar> prev() const
			{
				if (m_prev != '\0')
				{
					return m_prev;
				}
				return std::nullopt;
			}
			std::size_t left() const
			{
				return m_left;
			}
		 private:
			UChar m_prev{};
			UChar m_current{};
			std::string_view::iterator m_iter;
			std::size_t m_left;
		};
	 public:
		Chars(std::string_view view) : m_view{ view }
		{
		}
		iterator begin() const
		{
			return iterator{ m_view.begin(), U8Core::str_len(m_view)-1};
		}
		iterator end() const
		{
			return iterator{ m_view.end(), 0 };
		}
		std::string_view view() const
		{
			return m_view;
		}
	 protected:
		std::string_view m_view;
	};

}

#endif //UTF8STRING_SRC_CHAR_UCHAR_HPP_
