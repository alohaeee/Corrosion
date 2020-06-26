#ifndef CORROSION_SRC_PARSER_PARSER_HPP_
#define CORROSION_SRC_PARSER_PARSER_HPP_

#include "utility/std_incl.hpp"
#include "ast/ast.hpp"
#include "ast/assoc_prec.hpp"
#include "parse_session.hpp"
#include "token_stream.hpp"
#include "ast/token_tree.hpp"

using namespace corrosion::ast;
namespace corrosion
{
	struct TokenCursor
	{

	};
	class Parser
	{
	 private:
//		inline std::optional<ast::TokenKind> look(std::size_t n=0) noexcept
//		{
//			return m_tokenStream.lookahead(n);
//		}
//		inline std::optional<ast::Token> token(std::size_t n=0) noexcept
//		{
//			return m_tokenStream.token();
//		}
//		inline std::optional<ast::Token> shift() noexcept
//		{
//			return m_tokenStream.shift();
//		}
	 public:
//		void shift()
//		{
//		}
		bool check(TokenKind kind)
		{

		}


		Pointer<Expr> parseBlock();
		Pointer<Expr> parseBlockLine();

		Pointer<Expr> parseWhileExpr();
		Pointer<Expr> parseAssocExprWith(std::size_t minPrec, Pointer<Expr> lhs);
		Pointer<Expr> parsePrefixRangeExpr();
		Pointer<Expr> parsePrefixExpr();
		Pointer<Expr> parseDotOrCallExpr();
		Pointer<Expr> parseBottomExpr();
		Spanned<Pointer<Expr>> parsePrefixExprCommon(Span lo);
		Pointer<Expr> parseUnaryExpr(Span lo, UnOp op);

		//diagnistics

		std::optional<Pointer<Expr>> checkNoChainedComparison(Pointer<Expr> innerOp,Spanned<AssocOp> outerOp);


		TokenStreamV m_tokenStream{};
		ParseSession m_session;
		TokenStream tokens;


		/// The current token.
		Token token;
		/// The previous token.
		Token prevToken;
		std::vector<std::pair<TokenKind,std::optional<TokenData>>> expectedTokens;
		TokenCursor tokenCursor;

	 	//Item root;

	};
}

#endif //CORROSION_SRC_PARSER_PARSER_HPP_
