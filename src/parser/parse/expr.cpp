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
				//lhs = self.parse_assoc_op_cast(lhs, lhs_span, ExprKind::Cast)?;
				continue;
			}
			else if (op->kind == AssocOp::Colon)
			{
				//lhs = self.parse_assoc_op_ascribe(lhs, lhs_span)?;
				continue;
			}
			else if (op->kind == AssocOp::DotDot || op->kind == AssocOp::DotDotEq)
			{
				// If we didn’t have to handle `x..`/`x..=`, it would be pretty easy to
				// generalise it to the Fixity::None code.
				///lhs = self.parse_range_expr(prec, lhs, op, cur_op_span)?;
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
				session->errorSpan(lo,"TODO: can't parse borrow expr");
				//return this->parseBorrowExpr()
				return nullptr;
			}
		}
		case TokenKind::AndAnd:
			session->errorSpan(lo,"TODO: can't parse borrow expr");
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
//		if(token.kind == TokenKind::Literal)
//		{
//			//this->parseLitExpr();
//		}
		if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Paren}))
		{
			session->criticalSpan(token.span, "Maybe start of Tuple expr, but we can parse it now");
			return nullptr;
			//this->parseTupleParensExpr();
		}
		else if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Brace}))
		{
			return parseBlockExpr(std::nullopt, token.span);
		}
		else if(check(TokenKind::BinOp,data::BinOp{data::BinOp::Or})||check(TokenKind::OrOr))
		{
			session->criticalSpan(token.span, "Maybe start of Closure expr, but we can parse it now");
			return nullptr;
		}
		else if(check(TokenKind::Lt) || check(TokenKind::BinOp, data::BinOp{data::BinOp::Shl}))
		{
			session->criticalSpan(token.span, "Maybe start of QPath expr, but we can parse it now");
			return nullptr;
		}
		else if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Bracket}))
		{
			//this->parseArrayOrRepeatExpr();
		}
//		else if(eat_lit())
//		{
//			this->parseQPath();
//		}
		else if(checkPath())
		{
			//this->parseStartPathExpr();
		}
//		else if(checkKeyword(kw::Move) || checkKeyword(kw::Static))
//		{
//			//this->parseClosureExpr();
//		}
		else if(eatKeyword(kw::If))
		{
			//this->parseIfExpr();
		}
		else if(eatKeyword(kw::For))
		{
			//this->parseForExpr();
		}
		else if(eatKeyword(kw::While))
		{
			//this->parseWhileExpr();
		}
		else if(auto label = eatLabel(); label)
		{
			//this->parseLabelExpr();
		}
		else if(eatKeyword(kw::Loop))
		{
			//this->parseLoopExpr();
		}
		else if(eatKeyword(kw::Continue))
		{
			auto cont = ExprKind::Continue{eatLabel()};
			//
		}
		else if(eatKeyword(kw::Return))
		{
			//this->parseReturnExpr();
		}
		else if(eatKeyword(kw::Break))
		{
			//this->parseBreakExpr();
		}
//		else if(eatKeyword(kw::Let))
//		{
//			//this->parseLetExpr();
//		}
		else
		{

			return parseLitExpr();
		}
		return nullptr;

	}
	Pointer<Expr> Parser::parseLitExpr()
	{
		auto lo = token.span;
		auto lit = Literal::fromToken(token);
		this->shift();
		return MakePointer<Expr>(lo,std::move(lit));
	}
	Pointer<Expr> Parser::parseCondExpr()
	{
		return parseAssocExprWith(0,nullptr);
	}
	Pointer<Expr> Parser::parseWhileExpr()
	{
		auto cond = parseCondExpr();
		return nullptr;
	}

	Pointer<Expr> Parser::parseForExpr()
	{
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

}