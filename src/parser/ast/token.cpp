#include "token.hpp"

namespace corrosion::ast
{
	bool data::Ident::identCanBeginExpr(Span span, Symbol symbol)
	{
		auto ident_token = Token{ TokenKind::Ident, span, Ident{ symbol }};
		return Ident::exprKeyword(symbol);
	}
}
