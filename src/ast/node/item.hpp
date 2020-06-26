#ifndef CORROSION_SRC_AST_NODE_ITEM_HPP_
#define CORROSION_SRC_AST_NODE_ITEM_HPP_


#include "ast/fwd.hpp"

namespace corrosion
{
	struct Label
	{
		Ident ident;
	};

	struct Lifetime
	{
		NodeId id;
		Ident ident;
	};
	struct PathSegmenth
	{
		Ident ident;
		NodeId id;

		static PathSegmenth fromIdent(Ident ident)
		{

			return PathSegmenth{ ident, DUMMY_NODE_ID };
		}
		static PathSegmenth pathRoot(Span span)
		{
			return fromIdent(Ident{ kw::PathRoot, span });
		}
	};

	struct Path
	{
		Span span;
		std::vector<PathSegmenth> segments;

		// Convert a span and an identifier to the corresponding
		// one-segment path.
		static Path fromIdent(Ident ident)
		{
			return Path{ ident.span(), { PathSegmenth::fromIdent(ident) }};
		}
		bool isGlobal() const
		{
			return !segments.empty() && segments.at(0).ident.name() == kw::PathRoot;
		}
	};

	struct Block
	{
		std::vector<Stmt> stmts;
		NodeId id;
		Span span;
	};
}

#endif //CORROSION_SRC_AST_NODE_ITEM_HPP_
