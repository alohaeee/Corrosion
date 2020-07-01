#include "parser/parser.hpp"

namespace corrosion
{
	Pointer<Ty> Parser::parseTy()
	{
		Ty::KindUnion kind;
		auto lo = token.span;
		if(eat(TokenKind::OpenDelim, data::Delim{data::Delim::Bracket}))
		{
			kind = parseArrayTy();
		}
		else if(checkPath())
		{
			auto path = parsePath();
			kind = TyKind::Path{path};
		}
		else
		{
			session.info("Possibly that some original types of Rust are not implimented");
			session.criticalSpan(lo,"Looking for type but find:");
		}
		return MakePointer<Ty>(lo.to(prevToken.span),std::move(kind));
	}
	Ty::KindUnion Parser::parseArrayTy()
	{
		auto ty = parseTy();
		Ty::KindUnion kind;
		if(eat(TokenKind::Semi))
		{
			kind = TyKind::Array{ty,parseAnonConstExpr()};
		}
		else
		{
			session.errorSpan(ty->span,"Slice type is not supported for now");
			kind = TyKind::Slice{ty};
		}
		expect(TokenKind::CloseDelim,data::Delim{data::Delim::Bracket});
		return kind;
	}
}