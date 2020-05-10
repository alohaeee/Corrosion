#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <optional>
#include <string>
#include <type_traits>

#include "utility/uchar.hpp"

namespace corrosion
{

	template<typename char_type, char_type INVALID>
	class ASCIICursor
	{
	public:
		ASCIICursor() = default;
		virtual ~ASCIICursor() = default;
		explicit ASCIICursor(std::basic_string_view<char_type> text) : m_text(text)
		{}

		//! @brief Returns the last eaten symbol.
		char_type prev() const
		{
			if (m_consumed - 1 >= m_text.size())
			{
				return INVALID;
			}
			return m_text[m_consumed - 1];
		}

		//! @brief Checks if there is nothing more to consume.
		bool isEof() const
		{
			return m_consumed>=m_text.size();
		}

		//! @brief Returns amount of already consumed symbols.
		std::size_t consumed() const
		{
			return m_consumed;
		}

		//! @brief Returns amount of left symbols.
		std::size_t left() const
		{
			return m_text.size() - m_consumed;
		}

		/**
		 * @brief Returns nth character relative to the current cursor position.
		 */
		char_type look(std::size_t n) const
		{
			if (m_consumed + n >= m_text.size())
			{
				return INVALID;
			}
			return m_text[m_consumed + n];
		}

		//! @brief Peeks the next symbol from the input stream without consuming it.
		char_type first() const
		{
			return this->look(0);
		}

		//! @brief Peeks the second symbol from the input stream without consuming it.
		char_type second() const
		{
			return this->look(1);
		}

		//! @brief Peeks the third symbol from the input stream without consuming it.
		char_type third() const
		{
			return this->look(2);
		}

		//! @brief Return the text on which cursor points.
		auto text() const
		{
			return m_text;
		}

		//! @brief Moves to the next character and return consumed one.
		char_type shift()
		{
			m_consumed++;
			return prev();
		}
		//! @brief Checks whether character is valid.
		static bool isValid(char_type c)
		{
			return c != INVALID;
		}
		//! @brief Return invalid character.
		static char_type invalid()
		{
			return INVALID;
		}


	protected:
		std::basic_string_view<char_type> m_text{};
		std::size_t m_consumed{};
	};

	using wCursor = ASCIICursor<wchar_t, WEOF>;
	using Cursor = ASCIICursor<char, EOF>;


	template<char INVALID>
	class UTF8Cursor
	{
	 public:
		UTF8Cursor(std::string_view view) : m_chars{view}, m_begin{m_chars.begin()}, m_end{m_chars.end()},
		 m_size(U8Core::str_len(view))
		{}

		//! @brief Returns the last eaten symbol.
		UChar prev() const
		{
			auto prev = m_begin->prev();
			return prev.value_or(INVALID);
		}

		//! @brief Checks if there is nothing more to consume.
		bool isEof() const
		{
			return !left();
		}

		//! @brief Returns amount of already consumed symbols.

		std::size_t consumed() const
		{
			return m_size - left();
		}

		//! @brief Returns amount of left symbols.
		std::size_t left() const
		{
			return m_begin->left();
		}

		/**
		 * @brief Returns nth character relative to the current cursor position.
		 */
		UChar look(std::size_t n) const
		{
			if (this->consumed() + n > m_size())
			{
				return INVALID;
			}
			return *(m_begin + n);
		}

		//! @brief Peeks the next symbol from the input stream without consuming it.
		UChar first() const
		{
			return this->look(0);
		}

		//! @brief Peeks the second symbol from the input stream without consuming it.
		UChar second() const
		{
			return this->look(1);
		}

		//! @brief Peeks the third symbol from the input stream without consuming it.
		UChar third() const
		{
			return this->look(2);
		}

		//! @brief Return the text on which cursor points.
		const auto text() const
		{
			return m_chars.view();
		}

		//! @brief Moves to the next character and return consumed one.
		UChar shift()
		{
			m_begin++;
			return *m_begin;
		}
		//! @brief Checks whether character is valid.
		static bool isValid(UChar c)
		{
			return c != INVALID;
		}
		//! @brief Return invalid character.
		static UChar invalid()
		{
			return INVALID;
		}

	 protected:
		Chars m_chars;
		Chars::iterator m_begin;
		Chars::iterator m_end;
		const std::size_t m_size;
	};
	using u8Cursor = UTF8Cursor<EOF>;
} // namespace corrosion

#endif // CURSOR_HPP
