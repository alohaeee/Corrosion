#include "parser/parser.hpp"

namespace corrosion
{
	std::optional<Stmt> Parser::parseStmtWithoutRecovery()
	{
		auto lo = token.span;
		Stmt::KindUnion kind;
		if(eatKeyword(kw::Let))
		{
			kind = StmtKind::Local{parseLocal()};
		}
		else if(isKwFollowedByIdent(kw::Mut))
		{
			session->errorSpan(lo,"missing keyword let");
			kind = StmtKind::Local{parseLocal()};
		}
		else if(isKwFollowedByIdent(kw::Auto))
		{
			session->errorSpan(lo,"write `let` instead of `auto` to introduce a new variable");
			kind = StmtKind::Local{parseLocal()};
		}
		else if(isKwFollowedByIdent(sym::Var))
		{
			session->errorSpan(lo,"write `let` instead of `var` to introduce a new variable");
			kind = StmtKind::Local{parseLocal()};
		}
		else if(auto item = parseItemCommon(); item)
		{
			kind = StmtKind::Item{item};
		}
//		else if(checkPath())
//		{
//
//		}
		else if(eat(TokenKind::Semi))
		{
			kind = StmtKind::Empty{};
		}
		else if(!(token.kind ==TokenKind::CloseDelim && token.data == TokenData(data::Delim{data::Delim::Brace}))&&
			token.kind!=TokenKind::Eof)
		{
			auto e = parseExprRes(Restriction::STMT_EXPR);
			kind = StmtKind::Expr{e};
		}
		else
		{
			return std::nullopt;
		}
		return Stmt{lo.to(prevToken.span),std::move(kind),DUMMY_NODE_ID};
	}
	std::optional<Stmt> Parser::parseFullStmt()
	{
		auto stmt = parseStmtWithoutRecovery();
		bool eatSemi = true;
		if(stmt)
		{
			std::visit([&eatSemi, this, stmt](auto&& arg)
			{
			  using T = std::decay_t<decltype(arg)>;
			  if constexpr(std::is_same_v<T, StmtKind::Expr>)
			  {
				  if (arg.expr->requiresSemiToBeStmt() && token.kind != TokenKind::Eof)
				  {
					  if (!(check(TokenKind::Semi) || check(TokenKind::CloseDelim,data::Delim{data::Delim::Brace})))
					  {
						  session->errorSpan(token.span, "expected semicolon");
					  }
				  }
			  }
			  else if constexpr(std::is_same_v<T, StmtKind::Local>)
			  {
				  expect(TokenKind::Semi);
				  eatSemi = false;
			  }
			  else if constexpr(std::is_same_v<T, StmtKind::Empty>)
			  {
				  eatSemi = false;
			  }

			}, stmt->kind);
		}

		if(eatSemi && eat(TokenKind::Semi))
		{
			stmt->addTrailingSemicolon();
		}
		if(stmt)
		{
			stmt->span = stmt->span.to(prevToken.span);

		}

		return stmt;
	}
	Pointer<Local> Parser::parseLocal()
	{
		auto lo = prevToken.span;
		auto pat = parseTopPat(true);
		Pointer<Ty> ty;
		Pointer<Expr> ex;
		if(eat(TokenKind::Colon))
		{
			ty = parseTy();
		}
		if(eat(TokenKind::Eq))
		{
			ex = parseExpr();
		}
		return MakePointer<Local>(lo.to(prevToken.span),pat,ty,ex,DUMMY_NODE_ID);
	}
	Pointer<Block> Parser::parseBlockCommon()
	{
		auto lo = token.span;
		if(!eat(TokenKind::OpenDelim, data::Delim{data::Delim::Brace}))
		{
			session->errorSpan(token.span, "expected to meet {");
			return nullptr;
		}
		Pointer<Block> block = MakePointer<Block>();
		while(!eat(TokenKind::CloseDelim, data::Delim{data::Delim::Brace}))
		{
			if(token.kind ==TokenKind::Eof)
			{
				session->errorSpan(token.span, "expected to meet }");
				break;
			}
			auto stmt = parseFullStmt();
			block->stmts.push_back(std::move(stmt.value()));
		}
		block->span = lo.to(prevToken.span);

		return block;
	}

}