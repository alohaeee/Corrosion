#include "parser/parser.hpp"

namespace corrosion
{

	Pointer<Item> Parser::parseItemCommon()
	{
		auto lo = token.span;
		auto kind = parseItemKind(lo);
		if(kind)
		{
			return MakePointer<Item>(std::move(kind->first),lo.to(prevToken.span),std::move(kind->second));
		}
		else
		{
			return nullptr;
		}
	}
	std::optional<ItemInfo> Parser::parseItemKind(Span lo)
	{
		if (eatKeyword(kw::Use))
		{
			session->criticalSpan(lo.to(prevToken.span), "maybe start of Use item, but we can parse it now");
		}
		else if (checkFnFrontMatter())
		{
			auto&& [ident,sig,gen,block] = parseFn();
			return ItemInfo{std::move(ident),ItemKind::Fn{std::move(sig),block}};
		}
		else if (eatKeyword(kw::Extern))
		{
			session->criticalSpan(lo.to(prevToken.span), "maybe start of Extern item, but we can parse it now");
		}
		else if (checkKeyword(kw::Static))
		{
			shift();
			auto m = parseMutability();
			auto&& [ident,ty,expr] = parseItemGlobal(m);

			return ItemInfo{std::move(ident),ItemKind::Static{ty,m,expr}};
		}
		else if (auto constness = parseConstness();constness.kind == Const::Yes)
		{
			//const ITEM
			auto&& [ident,ty,expr] = parseItemGlobal(std::nullopt);
			return ItemInfo{std::move(ident),ItemKind::Const{ty,expr}};
		}
		else if (checkKeyword(kw::Trait))
		{
			session->criticalSpan(lo.to(token.span), "maybe start of Trait item, but we can parse it now");
		}
		else if (checkKeyword(kw::Impl))
		{
			session->criticalSpan(lo.to(token.span), "maybe start of Impl item, but we can parse it now");
		}
		else if (eatKeyword(kw::Mod))
		{
			session->criticalSpan(lo.to(prevToken.span), "maybe start of Mod item, but we can parse it now");
		}
		else if (eatKeyword(kw::Type))
		{
			session->criticalSpan(lo.to(prevToken.span), "maybe start of Type item, but we can parse it now");
		}
		else if (eatKeyword(kw::Enum))
		{
			session->criticalSpan(lo.to(prevToken.span), "maybe start of Enum item, but we can parse it now");
		}
		else if (eatKeyword(kw::Struct))
		{
			session->criticalSpan(lo.to(prevToken.span), "maybe start of Struct item, but we can parse it now");
		}
		else if (eatKeyword(kw::Union))
		{
			session->criticalSpan(lo.to(prevToken.span), "maybe start of Struct item, but we can parse it now");
		}
		else if (eatKeyword(kw::Macro))
		{
			session->criticalSpan(lo.to(prevToken.span), "maybe start of Macro item, but we can parse it now");
		}
		return std::nullopt;
	}

	bool Parser::checkFnFrontMatter()
	{
		auto constness = checkKeyword(kw::Const) && lookahead(1).isKeyword(kw::Fn);
		auto async = checkKeyword(kw::Async) && lookahead(1).isKeyword(kw::Fn);
		auto unsafe = checkKeyword(kw::Unsafe) && lookahead(1).isKeyword(kw::Fn);
		auto externFn = checkKeyword(kw::Extern) && lookahead(1).isKeyword(kw::Fn);
		auto externABIFn =
			checkKeyword(kw::Extern) && lookahead(1).canBeginLiteralMaybeMinus() && lookahead(2).isKeyword(kw::Fn);
		if (constness || async || unsafe || externFn || externABIFn)
		{
			session->criticalSpan(token.span, "maybe start of FnHeader, but we can parse it now");
		}
		return checkKeyword(kw::Fn) || constness ||async || unsafe || externFn || externABIFn;
	}
	std::tuple<Ident, FnSig, Generics, Pointer<Block>> Parser::parseFn()
	{
		//auto header = parseFnFrontMatter();
		if(!eatKeyword(kw::Fn))
		{
			session->criticalSpan(token.span,"expected `fn`");
		}
		auto ident = parseIdent();
		if(check(TokenKind::Lt) || check(TokenKind::BinOp, data::BinOp{data::BinOp::Shl}))
		{
			session->criticalSpan(token.span, "maybe start of Generics, but we can parse it now");
		}
		auto decl = parseFnDecl();
		auto body = parseFnBody();


		return {std::move(ident),FnSig{{}, decl}, Generics{}, body};
	}
	Pointer<FnDecl> Parser::parseFnDecl()
	{
		auto params = parseFnParams();
		auto ty = parseRetTy();
		return MakePointer<FnDecl>(std::move(params),ty);
	}

	std::vector<Param> Parser::parseFnParams()
	{
		std::vector<Param> params;
		bool first_param = true;
		expect(TokenKind::OpenDelim, data::Delim{data::Delim::Paren});
		if(!eat(TokenKind::CloseDelim, data::Delim{data::Delim::Paren}))
		{
			while (true)
			{
				auto param = parseParamGeneral(first_param);
				params.push_back(std::move(param));
				first_param = false;
				if(eat(TokenKind::CloseDelim, data::Delim{data::Delim::Paren}))
				{
					break;
				}
				else if (eat(TokenKind::Comma))
				{
					continue;
				}
				session->criticalSpan(token.span, "expect ',' or ')'");
				break;
			}
		}
		return params;
	}

	Param Parser::parseParamGeneral(bool firstParam)
	{
		auto lo = token.span;
		Pointer<Pat> pat = parseFnParamPat();
		expect(TokenKind::Colon);
		Pointer<Ty> ty = parseTy();
		return Param{pat,ty,lo.to(prevToken.span),false,DUMMY_NODE_ID};
	}
	std::optional<Param> Parser::parseSelfParam()
	{
		return std::optional<Param>();
	}
	Pointer<Block> Parser::parseFnBody()
	{
		if(eat(TokenKind::Semi))
		{
			session->errorSpan(token.span, "fn without body is not allowed for now");
			return nullptr;
		}
		else if(check(TokenKind::OpenDelim,data::Delim{data::Delim::Brace}))
		{
			return parseBlockCommon();
		}
		else
		{
			session->errorSpan(token.span, "expect ',' or fn body");
			return nullptr;
		}
	}

	std::tuple<Ident, Pointer<Ty>, Pointer<Expr>>
		Parser::parseItemGlobal(std::optional<Mutability> m)
	{
		auto ident = parseIdent();

		Pointer<Ty> ty;

		if(eat(TokenKind::Colon))
		{
			ty = parseTy();
		}
		else
		{
			session->errorSpan(token.span, "global item without type");
		}
		Pointer<Expr> expr;
		if(eat(TokenKind::Eq))
		{
			expr = parseExpr();
		}
		expect(TokenKind::Semi);

		return {ident,ty,expr};
	}


}