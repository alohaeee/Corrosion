#include "token.hpp"

namespace corrosion::ast
{
	namespace data
	{
		bool Ident::identCanBeginExpr(Span span, Symbol symbol)
		{
			auto ident_token = Token{ TokenKind::Ident, span, Ident{ symbol }};
			return Ident::exprKeyword(symbol);
		}
		bool Ident::identCanBeginType(Span span, Symbol symbol)
		{
			auto ident_token = Token{ TokenKind::Ident, span, Ident{ symbol }};
			return Ident::typeKeyword(symbol);
		}
	}
}
