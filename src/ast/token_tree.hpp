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
		[[nodiscard]] inline std::size_t size() const noexcept
		{
			return treeAndJoint.size();
		}
		[[nodiscard]] inline const TreeAndJoint& operator[](std::size_t n) const
		{
			return treeAndJoint[n];
		}

		std::vector<TreeAndJoint> treeAndJoint;
	};
	struct DelimSpan
	{
		DelimSpan(Span lhs,Span rhs) : open{lhs}, close{rhs}
		{}
		static DelimSpan Dummy()
		{
			return DelimSpan{{},{}};
		}
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
		TokenTree(const TokenTree &tree) : data{tree.data}
		{}
		TokenTree& operator=(const TokenTree& tree)
		{
			this->data = tree.data;
			return *this;
		}
		TokenTree& operator=(TokenTree&& tree)
		{
			this->data = std::move(tree.data);
			return *this;
		}
		static TokenTree openTT(DelimSpan span, ast::data::Delim kind) noexcept
		{
			return ast::Token{ ast::TokenKind::OpenDelim, span.open, kind };
		}
		static TokenTree closeTT(DelimSpan span, ast::data::Delim kind) noexcept
		{
			return ast::Token(ast::TokenKind::CloseDelim, span.close, kind);
		}

		std::variant<ast::Token, Delimited> data;
	};


	class TreeCursor
	{
	 public:
		[[nodiscard]] std::optional<TreeAndJoint> nextWithJoint() noexcept
		{
			if(m_index < stream.size())
			{
				return stream[m_index++];
			}
			return std::nullopt;
		}
		[[nodiscard]] std::optional<TreeAndJoint> lookahead(std::size_t n) const noexcept
		{
			if(n+m_index < stream.size())
			{
				return stream[n+m_index];
			}
			return std::nullopt;
		}
		TreeCursor(TokenStream&& stream) : stream{stream}
		{

		}
		TokenStream stream;
	 protected:
		std::size_t m_index{0};
	};

}

#endif //CORROSION_SRC_PARSER_AST_TOKEN_TREE_HPP_
