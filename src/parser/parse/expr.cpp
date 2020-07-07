#include "parser/parser.hpp"
#include "ast/node/expr.hpp"
#include "span/symbol.hpp"

namespace corrosion
{
	/// Checks if this expression is a successfully parsed statement.
	bool exprIsComplete(Pointer<Expr> e)
	{
		return !e->requiresSemiToBeStmt();
	}

	AnonConst Parser::parseAnonConstExpr()
	{
		return AnonConst{DUMMY_NODE_ID,parseExpr()};
	}
	Pointer<Expr> Parser::parseExpr()
	{
		return parseExprRes(Restriction::NO_RESTRICT);
	}
	Pointer<Expr> Parser::parseAssocExpr()
	{
		return parseAssocExprWith(0,nullptr);
	}
	Pointer<Expr> Parser::parseExprRes(Restriction res)
	{
		return withRes<Pointer<Expr>>(res,&Parser::parseAssocExpr);
	}

	Pointer<Expr> Parser::parseAssocExprWith(std::size_t minPrec, Pointer<Expr> lhs)
	{
		if (token.isRangeKind())
		{
			lhs = parsePrefixRangeExpr();
		}
		else
		{
			lhs = parsePrefixExpr();
		}

		if(!shouldContinueAsAssocExpr(lhs))
		{
			return lhs;
		}

		std::optional<AssocOp> op;
		this->expectedTokens.emplace_back(TokenKind::BinOp,data::BinOp{});
		for (;(op=checkAssocOp())&& op;)
		{
			auto cur_op_span = token.span;

			Restriction res;
			if(op->isAssignLike())
			{
				res = static_cast<Restriction>(this->restrictions & Restriction::NO_STRUCT_LITERAL);
			}
			else
			{
				res = this->restrictions;
			}

			auto prec = op->precedence();
			if (prec < minPrec)
			{
				break;
			}

			shift();
			if(op->isComparison())
			{
				if(this->checkNoChainedComparison(lhs,Spanned<AssocOp>{op.value(),cur_op_span}))
				{
					return lhs;
				}
			}


			// Special cases:
			if (op->kind == AssocOp::As)
			{
				session->criticalSpan(cur_op_span,"TODO: can't parse as cast");
				//lhs = self.parse_assoc_op_cast(lhs, lhs_span, ExprKind::Cast)?;
				continue;
			}
			else if (op->kind == AssocOp::Colon)
			{
				session->criticalSpan(cur_op_span,"TODO: can't parse ascribe");
				//lhs = self.parse_assoc_op_ascribe(lhs, lhs_span)?;
				continue;
			}
			else if (op->kind == AssocOp::DotDot || op->kind == AssocOp::DotDotEq)
			{
				// If we didn’t have to handle `x..`/`x..=`, it would be pretty easy to
				// generalise it to the Fixity::None code.
				///lhs = self.parse_range_expr(prec, lhs, op, cur_op_span)?;
				session->criticalSpan(cur_op_span,"TODO: can't parse dot's expressions");
				break;
			}
			auto fixity = op->fixity();
			uint8_t prec_adjustment;
			if(fixity == Fixity::Left || fixity == Fixity::None)
			{
				prec_adjustment = 1;
			}
			else
			{
				prec_adjustment = 0;
			}
			auto rhs = this->withRes<Pointer<Expr>>(static_cast<Restriction>(res - Restriction::STMT_EXPR),
				[prec,prec_adjustment,this]() -> Pointer<Expr>{
				return parseAssocExprWith(prec+prec_adjustment,nullptr);
			});


			auto span = Span::sum(lhs->span,rhs->span);
			if(auto ast_op = op->toAstBinOp();ast_op)
			{
				auto binary = ExprKind::Binary{ast_op.value(),lhs,rhs};
				lhs = MakePointer<Expr>(std::move(span),std::move(binary));
			}
			else
			{
				switch(op->kind)
				{
				case AssocOp::Assign:
					lhs = MakePointer<Expr>(std::move(span),ExprKind::Assign{lhs,rhs,cur_op_span});
					break;
				case AssocOp::AssignOp:
				{
					auto bin_op = BinOp::fromToken(op->optAssignOp.value());
					lhs = MakePointer<Expr>(std::move(span),ExprKind::AssignOp{bin_op,lhs,rhs});
					break;
				}
				case AssocOp::As:
				case AssocOp::Colon:
				case AssocOp::DotDot:
				case AssocOp::DotDotEq:
					session->criticalSpan(span,"COMPILER BUG: AssocOp should have been handled by special case");
					break;
				}
			}
			if(fixity == Fixity::None)
			{
				break;
			}
		}
		return lhs;
	}

	std::optional<AssocOp> Parser::checkAssocOp()
	{
		auto op = AssocOp::fromToken(token);
		return op;
	}
	bool Parser::shouldContinueAsAssocExpr(Pointer<Expr> &e)
	{
		auto complete = exprIsComplete(e);
		auto op = checkAssocOp();
		if(complete && !op)
		{
			return false;
		}
		if(!complete)
		{
			return true;
		}
	}
	Pointer<Expr> Parser::parsePrefixRangeExpr()
	{
		session->criticalSpan(token.span, "Range Expr are not implemented");
		return nullptr;
	}
	Pointer<Expr> Parser::parsePrefixExpr()
	{
		auto lo = token.span;
		switch(token.kind)
		{
		case TokenKind::Not:
			return this->parseUnaryExpr(lo,UnOpKind::Not); // `!expr`
		case TokenKind::BinOp:
		{
			auto data = token.getData<ast::data::BinOp>();
			switch(data.kind)
			{
			case data::BinOp::Minus:
				return this->parseUnaryExpr(lo,UnOpKind::Neg); // `-expr`
			case data::BinOp::Star:
				return this->parseUnaryExpr(lo,UnOpKind::Deref); // `*expr`
			case data::BinOp::And:
				session->criticalSpan(lo,"TODO: can't parse borrow expr");
				//return this->parseBorrowExpr()
				return nullptr;
			}
		}
		case TokenKind::AndAnd:
			session->criticalSpan(lo,"TODO: can't parse borrow expr");
			//return this->parseBorrowExpr()
			return nullptr;
		default:
			return this->parseDotOrCallExpr();
		}
	}

	Pointer<Expr> Parser::parseDotOrCallExpr()
	{
		auto base = this->parseBottomExpr();
		return parseDotOrCallExprWith(base,base->span);
	}
	Spanned<Pointer<Expr>> Parser::parsePrefixExprCommon(Span lo)
	{
		this->shift();
		auto ex = this->parsePrefixExpr();
		return {ex,lo.to(ex->span)};
	}
	Pointer<Expr> Parser::parseUnaryExpr(Span lo, UnOpKind op)
	{
		auto&& [ex,span] = parsePrefixExprCommon(lo);
		return MakePointer<Expr>(span,ExprKind::Unary{op,ex});
	}
	/// At the bottom (top?) of the precedence hierarchy,
	/// Parses things like parenthesized exprs, macros, `return`, etc.
	Pointer<Expr> Parser::parseBottomExpr()
	{
		auto lo = token.span;
		if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Paren}))
		{
			return this->parseTupleParensExpr();
		}
		else if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Brace}))
		{
			return parseBlockExpr(std::nullopt, token.span);
		}
		else if(check(TokenKind::BinOp,data::BinOp{data::BinOp::Or})||check(TokenKind::OrOr))
		{
			session->criticalSpan(token.span, "maybe start of Closure expr, but we can parse it now");
			return nullptr;
		}
		else if(check(TokenKind::Lt) || check(TokenKind::BinOp, data::BinOp{data::BinOp::Shl}))
		{
			session->criticalSpan(token.span, "maybe start of QPath expr, but we can parse it now");
			return nullptr;
		}
		else if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Bracket}))
		{
			return this->parseArrayOrRepeatExpr();
		}
		else if(checkPath())
		{
			return this->parseStartPathExpr();
		}
		else if(checkKeyword(kw::Move) || checkKeyword(kw::Static))
		{
			session->criticalSpan(token.span, "maybe start of Closure expr, but we can parse it now");
			return nullptr;
		}
		else if(eatKeyword(kw::If))
		{
			return this->parseIfExpr();
		}
		else if(eatKeyword(kw::For))
		{
			return this->parseForExpr(std::nullopt,lo);
		}
		else if(eatKeyword(kw::While))
		{
			return this->parseWhileExpr(std::nullopt,lo);
		}
		else if(auto label = eatLabel(); label)
		{
			return this->parseLabeledExpr(label.value());
		}
		else if(eatKeyword(kw::Loop))
		{
			return this->parseLoopExpr(std::nullopt,lo);
		}
		else if(eatKeyword(kw::Continue))
		{
			auto cont = ExprKind::Continue{eatLabel()};
			return MakePointer<Expr>(lo.to(prevToken.span), std::move(cont));
		}
		else if(eatKeyword(kw::Return))
		{
			return this->parseReturnExpr();
		}
		else if(eatKeyword(kw::Break))
		{
			return this->parseBreakExpr();
		}
		else if(eatKeyword(kw::Let))
		{
			session->criticalSpan(token.span, "maybe start of Let expr, but we can parse it now");
			//this->parseLetExpr();
		}
		else if (token.kind == TokenKind::Literal)
		{
			return this->parseLitExpr();
		}
		else
		{
			session->errorSpan(lo, "here must be expression");
			return MakePointer<Expr>(lo, ExprKind::Error{});
		}
		return nullptr;

	}
	Pointer<Expr> Parser::parseLitExpr()
	{
		auto lo = token.span;
		try
		{
			auto lit = Literal::fromToken(token);
			this->shift();
			return MakePointer<Expr>(lo,std::move(lit));
		}
		catch(LiteralError& err)
		{
			session->errorSpan(lo,err.toString());
			return MakePointer<Expr>(lo,ExprKind::Error{});
		}
	}
	Pointer<Expr> Parser::parseCondExpr()
	{
		auto cond = parseExprRes(Restriction::NO_STRUCT_LITERAL);
		return cond;
	}
	Pointer<Expr> Parser::parseWhileExpr(std::optional<Label> optLabel, Span lo)
	{
		auto cond = parseCondExpr();
		auto block = parseBlockCommon();
		return MakePointer<Expr>(lo.to(prevToken.span), ExprKind::While{cond,block,optLabel});
	}

	Pointer<Expr> Parser::parseForExpr(std::optional<Label> optLabel, Span lo)
	{
		session->criticalSpan(token.span, "maybe start of For expr, but we can parse it now");
		return corrosion::Pointer<Expr>();
	}
	Pointer<Expr> Parser::parseLitMaybeMinus()
	{
		auto lo = token.span;
		auto minus_present = eat(TokenKind::BinOp,data::BinOp{data::BinOp::Minus});
		auto lit = parseLitExpr();
		if(minus_present && lit)
		{
			return MakePointer<Expr>(lo.to(lit->span),ExprKind::Unary{ UnOpKind::Neg, lit});
		}
		return lit;
	}

	Pointer<Expr> Parser::parseDotOrCallExprWith(Pointer<Expr>& e, Span lo)
	{
		while(true)
		{
			if(eat(TokenKind::Question))
			{
				session->criticalSpan(prevToken.span, "Question exprs are not implemented!");
				continue;
			}
			if(eat(TokenKind::Dot))
			{
				session->criticalSpan(prevToken.span, "Method call are not implemented!");
				continue;
			}
			if(exprIsComplete(e))
			{
				expectedTokens.clear();
				return e;
			}
			if(token.kind == TokenKind::OpenDelim)
			{
				auto data = token.getData<data::Delim>();
				if(data.kind == data::Delim::Bracket)
				{
					e = parseIndexExpr(e,lo);
				}
				else if(data.kind == data::Delim::Paren)
				{
					e = parseFnCallExpr(e,lo);
				}
			}
		}
	}
	Pointer<Expr> Parser::parseFnCallExpr(Pointer<Expr>& e, Span lo)
	{
		assert(eat(TokenKind::OpenDelim,data::Delim{data::Delim::Paren}));
		std::vector<Pointer<Expr>> exprs;
		while(true)
		{
			auto expr = parseExpr();
			exprs.push_back(expr);
			if(eat(TokenKind::CloseDelim,data::Delim{data::Delim::Paren}))
			{
				break;
			}
			if(eat(TokenKind::Comma))
			{
				continue;
			}
			session->errorSpan(lo.to(expr->span), "Function call must have close paren ')'");
			break;
		}
		return MakePointer<Expr>(lo.to(prevToken.span),ExprKind::FunctionCall{e,std::move(exprs)});

	}
	Pointer<Expr> Parser::parseIndexExpr(Pointer<Expr>& e, Span lo)
	{
		assert(eat(TokenKind::OpenDelim,data::Delim{data::Delim::Bracket}));
		auto index = parseExpr();
		expect(TokenKind::CloseDelim,data::Delim{data::Delim::Bracket});
		return MakePointer<Expr>(lo.to(index->span),ExprKind::Index{e,index});
	}
	Pointer<Expr> Parser::parseBlockExpr(std::optional<Label> optLabel, Span lo)
	{
		auto block = parseBlockCommon();
		return MakePointer<Expr>(lo.to(block->span),ExprKind::Block{block,optLabel});
	}
	Pointer<Expr> Parser::parseTupleParensExpr()
	{
		auto lo = token.span;
		expect(TokenKind::OpenDelim,data::Delim{data::Delim::Paren});
		std::vector<Pointer<Expr>> exprs;
		while(true)
		{
			auto e = parseExpr();
			exprs.push_back(e);
			if(eat(TokenKind::Comma))
			{
				continue;
			}
			else if(eat(TokenKind::CloseDelim,data::Delim{data::Delim::Paren}))
			{
				break;
			}
			else
			{
				session->criticalSpan(token.span, "waited for comma or close paren ')', but found:");
			}
		}
		Expr::KindUnion kind;
		if(exprs.size() == 1)
		{
			kind = ExprKind::Paren{exprs.back()};
		}
		else
		{
			kind = ExprKind::Error{};
			session->errorSpan(lo.to(prevToken.span), "Tuple expressions are not allowed");
		}
		return MakePointer<Expr>(lo.to(prevToken.span), std::move(kind));
	}
	Pointer<Expr> Parser::parseArrayOrRepeatExpr()
	{
		auto lo = token.span;
		Expr::KindUnion kind;

		expect(TokenKind::OpenDelim,data::Delim{data::Delim::Bracket});

		if(eat(TokenKind::OpenDelim,data::Delim{data::Delim::Bracket}))
		{
			//empty vector
			kind = ExprKind::Array{};
		}
		else
		{
			// Non empty vector
			auto first_expr = parseExpr();
			if(eat(TokenKind::Semi))
			{
				// Repeating array syntax: `[ 0; 512 ]`
				auto count = parseAnonConstExpr();
				expect(TokenKind::CloseDelim,data::Delim{data::Delim::Bracket});
				kind = ExprKind::Error();
				session->errorSpan(lo.to(prevToken.span), "repeat expresion is not allowed for now");
			}
			else if(eat(TokenKind::Comma))
			{
				// Vector with two or more elements.
				std::vector<Pointer<Expr>> exprs{first_expr};
				while(true)
				{
					auto e = parseExpr();
					exprs.push_back(e);
					if(eat(TokenKind::Comma))
					{
						continue;
					}
					else if(eat(TokenKind::CloseDelim,data::Delim{data::Delim::Bracket}))
					{
						break;
					}
					else
					{
						session->criticalSpan(token.span, "waited for comma or close bracket '}', but found:");
					}
				}
				kind = ExprKind::Array{exprs};
			}
			else
			{
				// Vector with one element
				expect(TokenKind::CloseDelim,data::Delim{data::Delim::Bracket});
				kind = ExprKind::Array{std::vector<Pointer<Expr>>{first_expr}};
			}
		}

		return MakePointer<Expr>(lo.to(prevToken.span), std::move(kind));
	}
	Pointer<Expr> Parser::parseStartPathExpr()
	{
		auto lo = token.span;
		auto path = parsePath();

		return MakePointer<Expr>(lo.to(prevToken.span),ExprKind::Path{std::nullopt,path});
	}
	Pointer<Expr> Parser::parseIfExpr()
	{
		auto lo = token.span;
		auto cond = parseCondExpr();
		Pointer<Block> block;

		if(!cond->returns() || eatKeyword(kw::Else))
		{
			session->errorSpan(cond->span, "missing condition for `if` expression");
		}
		else
		{
			auto not_block = !(token.kind == TokenKind::OpenDelim
				&& token.data == TokenData{data::Delim{data::Delim::Brace}});

			if(not_block)
			{
				session->errorSpan(token.span, "this `if` expression has a condition, but no block");
			}
			else
			{
				block = parseBlockCommon();
			}
		}
		Pointer<Expr> elseExpr;
		if(eatKeyword(kw::Else))
		{
			elseExpr = parseElseExpr();
		}
		return MakePointer<Expr>(lo.to(prevToken.span),ExprKind::If{cond,block,elseExpr});
	}
	/// Parses an `else { ... }` expression (`else` token already eaten).
	Pointer<Expr> Parser::parseElseExpr()
	{
		// else span
		auto ctx_span = prevToken.span;
		Pointer<Expr> expr;
		if(eatKeyword(kw::If))
		{
			expr = parseIfExpr();
		}
		else
		{
			auto block = parseBlockCommon();
			expr = MakePointer<Expr>(block->span, ExprKind::Block{block,std::nullopt});
		}
		return expr;
	}
	Pointer<Expr> Parser::parseLoopExpr(std::optional<Label> optLabel, Span lo)
	{
		auto block = parseBlockCommon();
		return MakePointer<Expr>(lo.to(prevToken.span), ExprKind::Loop{block,optLabel});
	}
	/// Parse `'label: $expr`. The label is already parsed.
	Pointer<Expr> Parser::parseLabeledExpr(Label label)
	{
		auto lo = label.ident.span();
		Pointer<Expr> expr;
		if(!eat(TokenKind::Colon))
		{
			session->errorSpan(prevToken.span, "labeled expr must be followed by colon");
		}
		if(eatKeyword(kw::While))
		{
			expr = parseWhileExpr(label,lo);
		}
		else if(eatKeyword(kw::For))
		{
			expr = parseForExpr(label,lo);
		}
		else if(eatKeyword(kw::Loop))
		{
			expr = parseLoopExpr(label,lo);
		}
		else if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Brace}))
		{
			expr = parseBlockExpr(label,lo);
		}
		else
		{
			session->errorSpan(token.span,"expected `while`, `for`, `loop` or `{` after a label");
			// Continue as an expression in an effort to recover on `'label: non_block_expr`.
			expr = parseExpr();
		}
		return expr;
	}
	/// Parse `"return" expr?`.
	Pointer<Expr> Parser::parseReturnExpr()
	{
		auto lo = prevToken.span;
		Pointer<Expr> opt_expr;
		if(token.canBeginExpr())
		{
			opt_expr = parseExpr();
		}
		auto kind = ExprKind::Return{opt_expr};
		return MakePointer<Expr>(lo.to(prevToken.span), std::move(kind));
	}
	Pointer<Expr> Parser::parseBreakExpr()
	{
		auto lo = prevToken.span;
		auto label = eatLabel();
		Pointer<Expr> kind;
		auto not_block = !(token.kind == TokenKind::OpenDelim
			&& token.data == TokenData{data::Delim{data::Delim::Brace}});

		if(not_block || (this->restrictions & Restriction::NO_STRUCT_LITERAL) ==  Restriction::NO_STRUCT_LITERAL)
		{
			if(token.canBeginExpr())
			{
				kind = parseExpr();
			}
		}
		return MakePointer<Expr>(lo.to(prevToken.span), ExprKind::Break{label,kind});
	}

}