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
	struct TokenCursorFrame
	{
		ast::data::Delim delim;
		DelimSpan span;
		bool openDelim;
		TreeCursor cursor;
		bool closeDelim;

		TokenCursorFrame(data::Delim delim, DelimSpan&& span, TreeCursor stream) : openDelim{ delim.isEmpty()},
		closeDelim{delim.isEmpty()}, delim{delim}, span{span}, cursor{stream}
		{}
	};
	struct TokenCursor
	{
		Token next()
		{
			while(true)
			{
				TreeAndJoint tree{Token{},IsJoint::NotJoint};
				if(!this->frame.openDelim)
				{
					this->frame.openDelim = true;
					tree = {TokenTree::openTT(this->frame.span,this->frame.delim), IsJoint::NotJoint};
				}
				else if(auto next_tree = this->frame.cursor.nextWithJoint();next_tree)
				{
					tree = next_tree.value();
				}
				else if(!this->frame.closeDelim)
				{
					this->frame.closeDelim = true;
					tree = {TokenTree::closeTT(this->frame.span,this->frame.delim), IsJoint::NotJoint};
				}
				else if(stack.size())
				{
					this->frame = this->stack.back();
					this->stack.pop_back();
					continue;
				}
				else
				{
					return Token{TokenKind::Eof};
				}


				if(tree.first.isToken())
				{
					return tree.first.getToken();
				}
				else
				{
					auto delim = tree.first.getDelimited();
					this->stack.push_back(TokenCursorFrame{delim.kind,std::move(delim.span),std::move(delim.stream)});
				}
			}
		}
		std::vector<TokenCursorFrame> stack;
		TokenCursorFrame frame;
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
		Token nextToken()
		{
			return tokenCursor.next();
		}
	 public:
		void shiftWith(Token&& nextTok)
		{
			if(this->prevToken.kind == TokenKind::Eof)
			{
				this->m_session.criticalSpan(nextTok.span,"attempted to bump the parser past EOF (may be stuck in a loop)");
			}
			this->prevToken = std::move(this->token);
			this->token = nextTok;

			this->expectedTokens.clear();
		}
		void shift()
		{
			auto next_tok = nextToken();
			shiftWith(std::move(next_tok));
		}

		/// Checks if the next token is `tok`, and returns `true` if so.
		///
		/// This method will automatically add `tok` to `expected_tokens` if `tok` is not
		/// encountered.
		bool check(TokenKind kind, TokenData&& data = data::Empty{})
		{
			auto is_present = this->token.kind == kind && this->token.data == data;
			if(!is_present)
			{
				this->expectedTokens.emplace_back(kind,data);
			}
			return is_present;
		}
		bool checkOrExpected(bool ok,TokenKind kind, const TokenData& data = data::Empty{})
		{
			if(ok)
			{
				return true;
			}
			expectedTokens.emplace_back(kind,data);
			return false;
		}
		bool checkPath()
		{
			return checkOrExpected(token.isPathStart(), token.kind,token.data);
		}
		bool checkKeyword(SymType sym)
		{
			return token.isKeyword(sym);
		}

		/// Consumes a token 'tok' if it exists. Returns whether the given token was present.
		bool eat(TokenKind kind, TokenData&& data = data::Empty{})
		{
			auto is_present = check(kind,std::move(data));
			if(is_present)
			{
				this->shift();
			}
			return is_present;

		}
		bool eatKeyword(SymType sym)
		{
			if(this->checkKeyword(sym))
			{
				this->shift();
				return true;
			}
			return false;
		}
		std::optional<Label> eatLabel()
		{
			if(auto ident = token.lifetime();ident)
			{
				this->shift();
				return Label{ident.value()};
			}
			return std::nullopt;
		}


//
//		Pointer<Expr> parseBlock();
//		Pointer<Expr> parseBlockLine();
//
//		Pointer<Expr> parseWhileExpr();
		Pointer<Expr> parseAssocExprWith(std::size_t minPrec, Pointer<Expr> lhs);
		Pointer<Expr> parsePrefixRangeExpr();
		Pointer<Expr> parsePrefixExpr();
		Pointer<Expr> parseDotOrCallExpr();
		Pointer<Expr> parseBottomExpr();
		Spanned<Pointer<Expr>> parsePrefixExprCommon(Span lo);
		Pointer<Expr> parseUnaryExpr(Span lo, UnOp op);
		Pointer<Expr> parseLitExpr();

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
