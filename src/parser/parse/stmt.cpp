#include "parser/parser.hpp"

namespace corrosion
{
	Pointer<Stmt> Parser::parseStmtWithoutRecovery()
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
			//return parseLocal(lo
		}
		else if(isKwFollowedByIdent(kw::Auto))
		{
			session->errorSpan(lo,"write `let` instead of `auto` to introduce a new variable");
		}
		else if(isKwFollowedByIdent(sym::Var))
		{
			session->errorSpan(lo,"write `let` instead of `var` to introduce a new variable");
		}
//		else if(auto item = parseItemCommon(); parseItemCommon())
//		{
//
//		}
		else if(eat(TokenKind::Semi))
		{
			///PUSTOI
		}
//		else if(token.kind != TokenKind::CloseDelim && token.get)
//		{
///// PARSE EXPR BLOCK POKA NE CONEC.
//		}
		else
		{
			/// OSHIBKA
		}
		return MakePointer<Stmt>(lo.to(prevToken.span),std::move(kind),DUMMY_NODE_ID);
	}
	Pointer<Stmt> Parser::parseFullStmt()
	{
		auto stmt = parseStmtWithoutRecovery();
		bool eatSemi = true;
		std::visit([&eatSemi,this,stmt](auto&& arg)
		{
			using T = std::decay_t<decltype(arg)>;
			if constexpr(std::is_same_v<T,StmtKind::Expr>)
			{
				if(arg.expr->requiresSemiToBeStmt() && token.kind != TokenKind::Eof)
				{
					if(check(TokenKind::Semi))
					{
						session->errorSpan(token.span,"Expected semicolon, but found:");
					}
				}
			}
			else if constexpr(std::is_same_v<T,StmtKind::Local>)
			{
				expect(TokenKind::Semi);
				eatSemi = false;
			}
			else if constexpr(std::is_same_v<T,StmtKind::Empty>)
			{
				eatSemi = false;
			}

		},stmt->kind);

		if(eatSemi && eat(TokenKind::Semi))
		{
			stmt->addTrailingSemicolon();
		}
		stmt->span = stmt->span.to(prevToken.span);

		return corrosion::Pointer<Stmt>();
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
		if(!eat(TokenKind::OpenDelim, data::Delim{data::Delim::Brace}))
		{
			session->errorSpan(token.span, "Expected to meet {, but found:");
		}
		Pointer<Block> block = MakePointer<Block>();
		while(!eat(TokenKind::CloseDelim, data::Delim{data::Delim::Brace}))
		{
			if(token.kind ==TokenKind::Eof)
			{
				break;
			}
			auto stmt = parseFullStmt();
			//block->stmts.push_back()



		}


		//return ;
	}

}