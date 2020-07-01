#include "parser/parser.hpp"
#include "ast/node/expr.hpp"
#include "span/symbol.hpp"

namespace corrosion
{

//	Pointer<Expr> Parser::parseBlock()
//	{
//		while (look() == ast::TokenKind::OpenDelim)
//		{
//
//		}
//		return nullptr;
//	}
//	Pointer<Expr> Parser::parseBlockLine()
//	{
//
//		if (look() == ast::TokenKind::Lifetime)
//		{
//			auto lifetime = shift();
//			if (look() == ast::TokenKind::Ident)
//			{
//				auto ident = shift().value();
//				auto data = ident.getData<ast::data::Ident>();
//				auto symbol = data.symbol;
//				if (symbol.isKeyword())
//				{
//					switch (symbol.data())
//					{
//					case kw::While:
//						break;
//					case kw::Loop:
//						break;
//					case kw::For:
//						break;
//					}
//				}
//			}
//			m_session.errorSpan(lifetime->span, "expected while/for/loop after lifetime");
//			return nullptr;
//		}
//		else
//		{
//			if (auto looked = look().value();look())
//			{
//				switch (looked)
//				{
//				case ast::TokenKind::Semi:
//					// Return a NULL expression, nothing here.
//					return nullptr;
//				case ast::TokenKind::Ident:
//				{
//					auto ident = shift().value();
//					auto data = ident.getData<ast::data::Ident>();
//					auto symbol = data.symbol;
//					if (symbol.isKeyword())
//					{
//						switch (symbol.data())
//						{
//						case kw::While:
//							break;
//						case kw::Loop:
//							break;
//						case kw::For:
//							break;
//						case kw::If:
//							break;
//						case kw::Match:
//							break;
//						case kw::Return:
//						case kw::Break:
//						case kw::Continue:
//							break;
//
//						}
//					}
//				}
//
//				}
//			}
//
//		}
//		return nullptr;
//	}
//	Pointer<Expr> Parser::parseWhileExpr()
//	{
//		if (look() == ast::TokenKind::Ident)
//		{
//			auto ident = shift().value();
//			auto data = ident.getData<ast::data::Ident>();
//			auto symbol = data.symbol;
//			switch (symbol.data())
//			{
//				// let expr
//			case kw::Let:
//				m_session.warnSpan(token(-1)->span, "let pattern in while can not be parsed");
//			default:
//				break;
//				//ParseExpr()
//			}
//		}
//		return nullptr;
//	}
//

	Pointer<Expr> Parser::parseAssocExprWith(std::size_t minPrec, Pointer<Expr> lhs)
	{
		if (token.isRangeKind())
		{
			parsePrefixRangeExpr();
		}
		else
		{
			parsePrefixExpr();
		}

		std::optional<AssocOp> op;
		this->expectedTokens.emplace_back(TokenKind::BinOp,data::BinOp{});
		for (;(op=AssocOp::fromToken(token))&& op;)
		{
			auto cur_op_span = token.span;
			auto prec = op->precedence();
			if (prec < minPrec)
			{
				break;
			}

			shift();

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
			auto rhs = parseAssocExprWith(prec+prec_adjustment,nullptr);

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
					m_session.criticalSpan(span,"COMPILER BUG: AssocOp should have been handled by special case");
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
	Pointer<Expr> Parser::parsePrefixRangeExpr()
	{
		return nullptr;
	}
	Pointer<Expr> Parser::parsePrefixExpr()
	{
		auto lo = token.span;
		switch(token.kind)
		{
		case TokenKind::Not:
			return this->parseUnaryExpr(lo,UnOp::Not); // `!expr`
		case TokenKind::BinOp:
		{
			auto data = token.getData<ast::data::BinOp>();
			switch(data.kind)
			{
			case data::BinOp::Minus:
				return this->parseUnaryExpr(lo,UnOp::Neg); // `-expr`
			case data::BinOp::Star:
				return this->parseUnaryExpr(lo,UnOp::Deref); // `*expr`
			case data::BinOp::And:
				m_session.errorSpan(lo,"TODO: can't parse borrow expr");
				//return this->parseBorrowExpr()
				return nullptr;
			}
		}
		case TokenKind::AndAnd:
			m_session.errorSpan(lo,"TODO: can't parse borrow expr");
			//return this->parseBorrowExpr()
			return nullptr;
		default:
			return this->parseDotOrCallExpr();
		}
	}
	/// At the bottom (top?) of the precedence hierarchy,
	/// Parses things like parenthesized exprs, macros, `return`, etc.
	Pointer<Expr> Parser::parseDotOrCallExpr()
	{
		auto base = this->parseBottomExpr();
		return corrosion::Pointer<Expr>();
	}
	Spanned<Pointer<Expr>> Parser::parsePrefixExprCommon(Span lo)
	{
		this->shift();
		auto ex = this->parsePrefixExpr();
		return {ex,lo.to(ex->span)};
	}
	Pointer<Expr> Parser::parseUnaryExpr(Span lo, UnOp op)
	{
		auto&& [ex,span] = parsePrefixExprCommon(lo);
		return MakePointer<Expr>(span,ExprKind::Unary{op,ex});
	}
	Pointer<Expr> Parser::parseBottomExpr()
	{
		auto lo = token.span;
		if(token.kind == TokenKind::Literal)
		{
			//this->parseLitExpr();
		}
		else if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Paren}))
		{
			//this->parseTupleParensExpr();
		}
		else if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Brace}))
		{
			//this->parseBlockExpr();
		}
//		else if(check(TokenKind::BinOp,data::BinOp{data::BinOp::Or})||check(TokenKind::OrOr))
//		{
//			//this->parseClosureExpr();
//		}
//		else if(check(TokenKind::BinOp,data::BinOp{data::BinOp::Or})||check(TokenKind::OrOr))
//		{
//			//this->eatLt();
//		}
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
			return MakePointer<Expr>(lo.to(lit->span),ExprKind::Unary{UnOp::Neg,lit});
		}
		return lit;
	}

}