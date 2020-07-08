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
	enum Restriction
	{
		NO_RESTRICT = 0,
		STMT_EXPR = 1 << 0,
		NO_STRUCT_LITERAL = 1 << 1
	};
	struct TokenCursorFrame
	{
		ast::data::Delim delim;
		DelimSpan span;
		bool openDelim;
		TreeCursor cursor;
		bool closeDelim;

		TokenCursorFrame(data::Delim delim, DelimSpan&& span, TreeCursor&& stream) : openDelim{ delim.isEmpty() },
																					 closeDelim{ delim.isEmpty() },
																					 delim{ delim },
																					 span{ span },
																					 cursor{ stream }
		{
		}
	};
	struct TokenCursor
	{
		TokenCursor(TokenCursorFrame&& frame) : frame{ frame }
		{
		}
		Token next()
		{
			while (true)
			{
				TreeAndJoint tree{ Token{}, IsJoint::NotJoint };
				if (!this->frame.openDelim)
				{
					this->frame.openDelim = true;
					tree = { TokenTree::openTT(this->frame.span, this->frame.delim), IsJoint::NotJoint };
				}
				else if (auto next_tree = this->frame.cursor.nextWithJoint();next_tree)
				{
					tree = next_tree.value();
				}
				else if (!this->frame.closeDelim)
				{
					this->frame.closeDelim = true;
					tree = { TokenTree::closeTT(this->frame.span, this->frame.delim), IsJoint::NotJoint };
				}
				else if (stack.size())
				{
					this->frame = this->stack.back();
					this->stack.pop_back();
					continue;
				}
				else
				{
					return Token{ TokenKind::Eof };
				}

				if (tree.first.isToken())
				{
					return tree.first.getToken();
				}
				else
				{
					auto delim = tree.first.getDelimited();
					this->stack.push_back(this->frame);
					this->frame = TokenCursorFrame{ delim.kind, std::move(delim.span),
													std::move(delim.stream) };

				}
			}
		}
		TokenCursorFrame frame;
		std::vector<TokenCursorFrame> stack{};
	};
	class Parser
	{
	 private:
		Token nextToken()
		{
			return tokenCursor.next();
		}
	 public:
		Parser(std::shared_ptr<ParseSession>& sess, TokenStream&& stream) : session(sess),
																			tokenCursor(TokenCursorFrame{
																				data::Delim{ data::Delim::NoDelim },
																				DelimSpan::Dummy(), std::move(stream) })
		{
			shift();
		}
		void shiftWith(Token&& nextTok)
		{
			if (this->prevToken.kind == TokenKind::Eof)
			{
				this->session->criticalSpan(nextTok.span,
					"attempted to bump the parser past EOF (may be stuck in a loop)");
			}
			this->prevToken = std::move(this->token);
			this->token = nextTok;
			CR_DEBUG_LOG_TRACE("token:{}", token.printable());

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
			if (!is_present)
			{
				this->expectedTokens.emplace_back(kind, data);
			}
			return is_present;
		}
		bool checkOrExpected(bool ok, TokenKind kind, const TokenData& data = data::Empty{})
		{
			if (ok)
			{
				return true;
			}
			expectedTokens.emplace_back(kind, data);
			return false;
		}
		bool checkPath()
		{
			return checkOrExpected(token.isPathStart(), token.kind, token.data);
		}
		bool checkKeyword(SymType sym)
		{
			return token.isKeyword(sym);
		}

		/// Consumes a token 'tok' if it exists. Returns whether the given token was present.
		bool eat(TokenKind kind, TokenData&& data = data::Empty{})
		{
			auto is_present = check(kind, std::move(data));
			if (is_present)
			{
				this->shift();
			}
			return is_present;

		}
		bool eatKeyword(SymType sym)
		{
			if (this->checkKeyword(sym))
			{
				this->shift();
				return true;
			}
			return false;
		}
		std::optional<Label> eatLabel()
		{
			if (auto ident = token.lifetime();ident)
			{
				this->shift();
				return Label{ ident.value() };
			}
			return std::nullopt;
		}
		Token lookahead(std::size_t n)
		{
			auto tree = tokenCursor.frame.cursor.lookahead(n);
			if (tree)
			{
				if (tree->first.isToken())
				{
					return tree->first.getToken();
				}
				else
				{
					auto delim = tree->first.getDelimited();
					return Token{ TokenKind::OpenDelim, delim.span.close, delim.kind };
				}

			}
			return Token{ TokenKind::CloseDelim, tokenCursor.frame.span.close, tokenCursor.frame.delim };

		}
		/// Is the given keyword `kw` followed by a non-reserved identifier?
		bool isKwFollowedByIdent(SymType kw)
		{
			return token.isKeyword(kw) && !lookahead(1).isKeyword();
		}

		Mutability parseMutability()
		{
			if (eatKeyword(kw::Mut))
			{
				return Mutability::Mut;
			}
			return Mutability::Not;
		}
		Ident parseIdent()
		{
			if (token.isIdent())
			{
				auto ident = Ident{ token.getData<data::Ident>().symbol, token.span };
				this->shift();
				return ident;
			}
			session->criticalSpan(token.span, "trying to eat ident, but find: ");
			return Ident{};
		}

		Const parseConstness()
		{
			if(eatKeyword(kw::Const))
			{
				return Const{prevToken.span,Const::Yes};
			}
			else
			{
				return Const{std::nullopt, Const::No};
			}
		};

		void expect(TokenKind kind, TokenData&& data = data::Empty{})
		{
			if (check(kind,std::move(data)))
			{
				this->shift();
				return;
			}
			if (expectedTokens.size() == 1)
			{
				Span span = token.span;
				if (token.kind == TokenKind::Eof)
				{
					span = prevToken.span;
				}
				session->criticalSpan(span,
					fmt::format("expected token: {}", Token{ kind, {}, data }.printable()));
			}
			else
			{
				std::string msg;
				for (auto &&[kind, data]: expectedTokens)
				{
					msg += (fmt::format("'{}' or ", Token{ kind, {}, data }.printable()));
				}
				msg.pop_back();
				msg.pop_back();
				msg.pop_back();
				session->criticalSpan(token.span, fmt::format("expected: {}", msg));
			}
		}

		template<typename Re>
		Re withRes(Restriction res, std::function<Re(Parser&)> func)
		{
			auto old = this->restrictions;
			this->restrictions = res;
			auto result = std::invoke(func, *this);
			this->restrictions = old;

			return result;
		}
		template<typename Re>
		Re withRes(Restriction res, std::function<Re()> func)
		{
			auto old = this->restrictions;
			this->restrictions = res;
			auto result = std::invoke(func);
			this->restrictions = old;

			return result;
		}

		inline void printAst(std::vector<Stmt>& stmts)
		{
#ifdef CR_ENABLE_LOG_AST
			CR_LOG_AST("![AST in file: {}]", session->file().path().string());
			for(auto&stmt: stmts)
			{
				stmt.printer(0);
			}
			Log::getAstLogger()->flush();
#endif
		}

		/* EXPR */
		bool exprIsComplete(Pointer<Expr>& e);
		Pointer<Expr> parseExpr();
		inline Pointer<Expr> parseExprRes(Restriction res);
		inline Pointer<Expr> parseAssocExpr();
		Pointer<Expr> parseAssocExprWith(std::size_t minPrec, Pointer<Expr> lhs);
		bool shouldContinueAsAssocExpr(Pointer<Expr>& e);
		std::optional<AssocOp> checkAssocOp();
		Pointer<Expr> parsePrefixRangeExpr();
		Pointer<Expr> parsePrefixExpr();
		Pointer<Expr> parseDotOrCallExpr();
		Pointer<Expr> parseDotOrCallExprWith(Pointer<Expr>& e, Span lo);
		Pointer<Expr> parseBottomExpr();
		Spanned<Pointer<Expr>> parsePrefixExprCommon(Span lo);
		Pointer<Expr> parseUnaryExpr(Span lo, UnOpKind op);
		Pointer<Expr> parseLitExpr();
		Pointer<Expr> parseLitMaybeMinus();
		Pointer<Expr> parseCondExpr();
		Pointer<Expr> parseWhileExpr(std::optional<Label> optLabel, Span lo);
		Pointer<Expr> parseLoopExpr(std::optional<Label> optLabel, Span lo);
		Pointer<Expr> parseTupleParensExpr();
		Pointer<Expr> parseBlockExpr(std::optional<Label> optLabel, Span lo);
		Pointer<Expr> parseArrayOrRepeatExpr();
		Pointer<Expr> parseStartPathExpr();
		Pointer<Expr> parseReturnExpr();
		Pointer<Expr> parseBreakExpr();
		Pointer<Expr> parseForExpr(std::optional<Label> optLabel, Span lo);
		Pointer<Expr> parseIfExpr();
		Pointer<Expr> parseElseExpr();

		Pointer<Expr> parseLabeledExpr(Label label);
		Pointer<Expr> parseFnCallExpr(Pointer<Expr>& e, Span lo);
		Pointer<Expr> parseIndexExpr(Pointer<Expr>& e, Span lo);
		AnonConst parseAnonConstExpr();

		/* STMT */
		Pointer<Block> parseBlockCommon();
		std::optional<Stmt> parseStmtWithoutRecovery();
		Pointer<Local> parseLocal();
		std::optional<Stmt> parseFullStmt();

		/* PATTERNS */
		inline Pointer<Pat> parsePat();
		Pointer<Pat> parseTopPat(bool gateOr);
		Pointer<Pat> parsePatWithOr(bool gateOr);
		Pointer<Pat> parsePatWithRangePat(bool allowRangePat);
		PatKind::Ref parsePatDeref();
		PatKind::Paren parsePatParen();
		PatKind::Ident parsePatIdentRef();
		PatKind::Ident parsePatIdent(BindingMode bindingMode);
		Pointer<Pat> parseFnParamPat();


		/* Types */
		Pointer<Ty> parseTy();
		Ty::KindUnion parseArrayTy();
		Pointer<Ty> parseRetTy();

		/* Items */
		Pointer<Item> parseItemCommon();
		std::tuple<Ident,Pointer<Ty>, Pointer<Expr>> parseItemGlobal(std::optional<Mutability> m);
		std::optional<ItemInfo> parseItemKind(Span lo);
		bool checkFnFrontMatter();
		std::tuple<Ident,FnSig,Generics,Pointer<Block>> parseFn();
		Pointer<FnDecl> parseFnDecl();
		std::vector<Param> parseFnParams();
		Param parseParamGeneral(bool firstParam);
		std::optional<Param> parseSelfParam();
		Pointer<Block> parseFnBody();



		/* Path */
		Path parsePath();
		void parsePathSegments(std::vector<PathSegment>& segments);
		Ident parsePathSegmentIdent();

		//diagnistics

		bool checkNoChainedComparison(Pointer<Expr> innerOp, Spanned<AssocOp>&& outerOp);

		std::shared_ptr<ParseSession> session;
		/// The current token.
		Token token{};
		/// The previous token.
		Token prevToken{};
		std::vector<std::pair<TokenKind, TokenData>> expectedTokens{};
		TokenCursor tokenCursor;
		Restriction restrictions = NO_RESTRICT;

		//Item root;

	};
}

#endif //CORROSION_SRC_PARSER_PARSER_HPP_
