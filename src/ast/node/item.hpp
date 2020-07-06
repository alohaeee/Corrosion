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
	struct PathSegment
	{
		Ident ident;
		NodeId id;

		static PathSegment fromIdent(Ident ident)
		{

			return PathSegment{ ident, DUMMY_NODE_ID };
		}
		static PathSegment pathRoot(Span span)
		{
			return fromIdent(Ident{ kw::PathRoot, span });
		}
	};

	struct Path
	{
		Span span;
		std::vector<PathSegment> segments;

		// Convert a span and an identifier to the corresponding
		// one-segment path.
		static Path fromIdent(Ident ident)
		{
			return Path{ ident.span(), { PathSegment::fromIdent(ident) }};
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

		void printer (std::size_t level)
	};


	struct Param
	{
		Pointer<Ty> type;
		Pointer<Pat> pat;
		NodeId id;
		Span span;
		bool isPlaceholder;
	};
	struct FnHeader
	{

	};
	struct FnDecl
	{
		std::vector<Param> param;
		Pointer<Ty> returnType;
	};
	struct FnSig
	{
		FnHeader header;
		Pointer<FnDecl> decl;
	};
	struct ItemKind
	{
		/// A constant item (`const`).
		///
		/// E.g., `const FOO: i32 = 42;`.
		struct Const
		{
			Pointer<Ty> type;
			std::optional<Pointer<Expr>> expr;
		};
		/// A function declaration (`fn`).
		///
		/// E.g., `fn foo(bar: usize) -> usize { .. }`.
		struct Fn
		{
			FnSig sig;
			std::optional<Pointer<Block>> block;
		};
	};
	struct Item
	{
		NodeId id;
		Span span;
		using KindUnion = std::variant<ItemKind::Const,ItemKind::Fn>;
		KindUnion kind;
		//Visibility vis;
		/// The name of the item.
		/// It might be a dummy name in case of anonymous items.
		Ident ident;

//		std::string printer(std::unique_ptr<Printer>& prevPrinter)
//		{
//			if(ETO IF)
//			{
//				"node If id = 1 (2,3)"
//	""				cond
//					{
//						block
//					}
//				prevPrinter.next = std::make_unique<Printer>()
//				    prevPrinter.child = printer()
//				printer(prevPrinter.next);
//			}
//		}
//		struct Printer
//		{
//			std::string content;
//			std::unique_ptr<Printer> next;
//			std::unique_ptr<Printer> child;
//			static inline std::size_t curLevel = 0;
//
//			Printer(std::string label, Span span, NodeId id)
//			{
//				label = fmt::format("Node {} id = ({},{})", label, (id == DUMMY_NODE_ID ? "DUMMY" : std::to_string(id)),
//					span.lo(),span.hi());
//			}
//			std::string print(std::size_t level)
//			{
//				if(child)
//				{
//					print(level+1);
//				}
//				if(next)
//				{
//					print(level);
//				}
				//
//			}
		//};
	};
}

#endif //CORROSION_SRC_AST_NODE_ITEM_HPP_
