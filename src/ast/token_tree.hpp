#ifndef CORROSION_SRC_PARSER_AST_TOKEN_TREE_HPP_
#define CORROSION_SRC_PARSER_AST_TOKEN_TREE_HPP_

#include "utility/std_incl.hpp"
#include "ast/token.hpp"

namespace corrosion
{
	enum class IsJoint
	{
		Joint,
		NotJoint
	};
	struct TokenTree;
	using TreeAndJoint = std::pair<TokenTree,IsJoint>;
	struct TokenStream
	{
		void push(TreeAndJoint&& tree);
		std::vector<TreeAndJoint> treeAndJoint;
	};
	struct DelimSpan
	{
		DelimSpan(Span lhs,Span rhs) : open{lhs}, close{rhs}
		{}
		inline Span entire() noexcept
		{
			return Span::sum(open, close);
		}
		Span open;
		Span close;
	};
	struct Delimited
	{
		DelimSpan span;
		ast::data::Delim kind;
		TokenStream stream;
	};
	struct TokenTree
	{
		auto getToken()
		{
			return std::get<ast::Token>(data);
		}
		auto getDelimited()
		{
			return std::get<Delimited>(data);
		}
		const auto getToken() const
		{
			return std::get<ast::Token>(data);
		}
		const auto getDelimited() const
		{
			return std::get<Delimited>(data);
		}
		inline bool isToken() const noexcept
		{
			return std::holds_alternative<ast::Token>(data);
		}
		inline bool isDelimited() const noexcept
		{
			return std::holds_alternative<Delimited>(data);
		}
		Span span()
		{
			if (isToken())
			{
				return getToken().span;
			}
			else
			{
				return getDelimited().span.entire();
			}
		}
		TokenTree(ast::Token token) : data{ token }
		{
		}
		TokenTree(Delimited delimited) : data{delimited}
		{

		}
		TokenTree openTT(DelimSpan span, ast::data::Delim kind)
		{
			return ast::Token{ ast::TokenKind::OpenDelim, span.open, kind };
		}
		TokenTree closeTT(DelimSpan span, ast::data::Delim kind)
		{
			return ast::Token(ast::TokenKind::CloseDelim, span.close, kind);
		}

		std::variant<ast::Token, Delimited> data;
	};


	class StreamCursor
	{

//		shift()
//		{
//
//		}
		TokenStream stream;
	 protected:
		std::size_t m_index;

	};

}

#endif //CORROSION_SRC_PARSER_AST_TOKEN_TREE_HPP_
