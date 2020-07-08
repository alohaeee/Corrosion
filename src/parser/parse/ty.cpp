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
			session->warn("possibly that some original types of Rust are not implemented");
			session->criticalSpan(lo,"looking for type, but find");
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
			session->errorSpan(ty->span,"slice type is not supported for now");
			kind = TyKind::Slice{ty};
		}
		expect(TokenKind::CloseDelim,data::Delim{data::Delim::Bracket});
		return kind;
	}
	Pointer<Ty> Parser::parseRetTy()
	{
		if(eat(TokenKind::RArrow))
		{
			auto type = this->parseTy();
			return type;
		}
		return nullptr;
	}
}