#ifndef CORROSION_SRC_AST_NODE_ITEM_HPP_
#define CORROSION_SRC_AST_NODE_ITEM_HPP_

#include "ast/fwd.hpp"

namespace corrosion
{
	struct Label
	{
		Ident ident;
	};
	struct Generics
	{

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

	struct Const
	{
		std::optional<Span> span;
		enum
		{
			Yes,
			No
		} kind;
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

		void printer(std::size_t level);
	};

	struct Param
	{
		Pointer<Pat> pat;
		Pointer<Ty> type;
		Span span;
		bool isPlaceholder;
		NodeId id;

		void printer(std::size_t level);
	};
	struct FnHeader
	{

	};
	struct FnDecl
	{
		std::vector<Param> param;
		Pointer<Ty> returnType;

		FnDecl(std::vector<Param>&& param, Pointer<Ty> returnType) : param{param}, returnType{returnType}
		{
		}

		void printer(std::size_t level);
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
			Pointer<Expr> expr;
		};
		/// A static item (`static`).
		///
		/// E.g., `static FOO: i32 = 42;` or `static FOO: &'static str = "bar";`.
		struct Static
		{
			Pointer<Ty> type;
			Mutability mut;
			Pointer<Expr> expr;
		};
		/// A function declaration (`fn`).
		///
		/// E.g., `fn foo(bar: usize) -> usize { .. }`.
		struct Fn
		{
			FnSig sig;
			Pointer<Block> block;
		};
		struct Error
		{

		};
	};

	struct Item
	{
		NodeId id;
		Span span;
		using KindUnion = std::variant<ItemKind::Const, ItemKind::Fn, ItemKind::Static>;
		KindUnion kind;
		//Visibility vis;
		/// The name of the item.
		/// It might be a dummy name in case of anonymous items.
		Ident ident;

		Item(Ident&& ident,Span span, KindUnion&& kind, NodeId id = DUMMY_NODE_ID):
			ident{ident}, span{span}, kind{kind}, id{id}
		{
		}

		void printer(std::size_t level);
	};
	using ItemInfo = std::pair<Ident,Item::KindUnion>;
}

#endif //CORROSION_SRC_AST_NODE_ITEM_HPP_
