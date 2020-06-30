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
//		else if(isKwFollowedByIdent(kw::Mut))
//		{
//			//m_session.errorSpan(lo)
//			//return parseLocal(lo
//		}
//		else if(isKwFollowedByIdent(kw::Auto))
//		{
//
//		}
//		else if(isKwFollowedByIdent(sym::Var))
//		{
//
//		}
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



	}
}