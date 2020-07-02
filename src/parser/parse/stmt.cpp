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
		expect(TokenKind::Semi);
		return MakePointer<Local>(lo.to(prevToken.span),pat,ty,ex,DUMMY_NODE_ID);
	}
	Pointer<Block> Parser::parseBlockCommon()
	{
		if(!eat(TokenKind::OpenDelim, data::Delim{data::Delim::Brace}))
		{
			session->errorSpan(token.span, "Expected to meet {, but found:");
		}
		while(!eat(TokenKind::CloseDelim, data::Delim{data::Delim::Brace}))
		{
			if(token.kind ==TokenKind::Eof)
			{
				break;
			}

		}


		return nullptr;
	}
}