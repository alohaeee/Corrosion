#include "parser/parser.hpp"

namespace corrosion
{
	Pointer<Stmt> Parser::parseStmtWithoutRecovery()
	{
		auto lo = token.span;
		if(eatKeyword(kw::Let))
		{
			//return parseLocal(lo)
		}
		else if(isKwFollowedByIdent(kw::Mut))
		{
			m_session.errorSpan(lo,"missing keyword let");
			//return parseLocal(lo
		}
		else if(isKwFollowedByIdent(kw::Auto))
		{
			m_session.errorSpan(lo,"write `let` instead of `auto` to introduce a new variable");
		}
		else if(isKwFollowedByIdent(sym::Var))
		{
			m_session.errorSpan(lo,"write `let` instead of `var` to introduce a new variable");
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
		return nullptr;
	}
	Pointer<Local> Parser::parseLocal()
	{
		auto lo = prevToken.span;
		auto pat = parseTopPat(true);
		if(eat(TokenKind::Colon))
		{
		}
		return nullptr;
	}
	Pointer<Block> Parser::parseBlockCommon()
	{
		if(!eat(TokenKind::OpenDelim, data::Delim{data::Delim::Brace}))
		{
			m_session.errorSpan(token.span, "Expected to meet {, but found:");
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