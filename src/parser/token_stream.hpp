#ifndef CORROSION_SRC_PARSER_TOKENS_STREAM_HPP_
#define CORROSION_SRC_PARSER_TOKENS_STREAM_HPP_

#include "utility/std_incl.hpp"
#include "ast/token.hpp"

namespace corrosion
{
	class TokenStreamV
	{
		bool isValid(std::size_t index) const
		{
			return index+m_current < m_data.size();
		}
	 public:
		std::optional<ast::TokenKind> lookahead(std::size_t n=0) const noexcept
		{
			if(isValid(n))
			{
				return m_data[n+m_current].kind;
			}
			return std::nullopt;
		}
		std::optional<ast::Token> token(std::size_t n=0) const noexcept
		{
			if(isValid(n))
			{
				return m_data[n+m_current];
			}
			return std::nullopt;
		}
		std::optional<ast::Token> shift() noexcept
		{
			auto tok = token();
			m_current++;
			return tok;
		}
		std::optional<ast::Token> shiftBack() noexcept
		{
			auto tok = token();
			m_current--;
			return tok;
		}
		void pushBack(ast::Token token)
		{
			m_data.push_back(token);
		}
	 private:
		std::size_t m_current{0};
		std::vector<ast::Token> m_data{};
	};
}

#endif //CORROSION_SRC_PARSER_TOKENS_STREAM_HPP_
