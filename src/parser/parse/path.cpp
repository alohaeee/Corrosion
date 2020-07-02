#include "parser/parser.hpp"
namespace corrosion
{
	Path Parser::parsePath()
	{
		auto lo = token.span;
		std::vector<PathSegment> segments;
		if(eat(TokenKind::ModSep))
		{
			segments.push_back(PathSegment::pathRoot(lo));
		}
		parsePathSegments(segments);
		return Path{lo.to(prevToken.span),segments};
	}

	void Parser::parsePathSegments(std::vector<PathSegment>& segments)
	{
		while(true)
		{
			auto seg = PathSegment::fromIdent(parsePathSegmentIdent());

			segments.emplace_back(std::move(seg));
			if(!eat(TokenKind::ModSep))
			{
				if(token.isQPathStart())
				{
					session->criticalSpan(token.span, "Generic in path start are not implimented");
				}
				return;
			}

		}

	}

	Ident Parser::parsePathSegmentIdent()
	{
		if(token.isIdent())
		{
			auto data = token.getData<data::Ident>();
			if(data.symbol.isPathSegment())
			{
				this->shift();
				return {data.symbol,prevToken.span};
			}
		}
		return parseIdent();
	}
}