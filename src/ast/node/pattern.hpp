#ifndef CORROSION_SRC_AST_NODE_PATTERN_HPP_
#define CORROSION_SRC_AST_NODE_PATTERN_HPP_

#include "ast/fwd.hpp"

namespace corrosion
{

	enum class Mutability
	{
		Mut,
		Not,
	};
	enum class BorrowKind
	{
		/// A normal borrow, `&$expr` or `&mut $expr`.
		/// The resulting type is either `&'a T` or `&'a mut T`
		/// where `T = typeof($expr)` and `'a` is some lifetime.
		Ref,
		/// A raw borrow, `&raw const $expr` or `&raw mut $expr`.
		/// The resulting type is either `*const T` or `*mut T`
		/// where `T = typeof($expr)`.
		Raw
	};

	struct BindingMode
	{
		enum
		{
			ByValue,
			ByRef
		}kind;
		Mutability mut;
	};
	struct PatKind
	{
		/// Represents a wildcard pattern (`_`).
		struct Wild
		{
		};
		/// A `PatKind::Ident` may either be a new bound variable (`ref mut binding @ OPT_SUBPATTERN`),
		/// or a unit struct/variant pattern, or a const pattern (in the last two cases the third
		/// field must be `None`). Disambiguation cannot be done with parser alone, so it happens
		/// during name resolution.
		struct Ident
		{
			BindingMode bindingMode;
			corrosion::Ident ident;
			std::optional<Pointer<Pat>> pat;
		};
		/// An or-pattern `A | B | C`.
		/// Invariant: `pats.len() >= 2`.
		struct Or
		{
			std::vector<Pointer<Pat>> pats;
		};
		/// A reference pattern (e.g., `&mut (a, b)`).
		struct Ref
		{
			Pointer<Pat> pat;
			Mutability mut;
		};
		struct Literal
		{
			Pointer<Expr> expr;
		};

		struct Path
		{

		};
		/// Parentheses in patterns used for grouping (i.e., `(PAT)`).
		struct Paren
		{
			Pointer<Pat> pat;
		};
	};
	struct Pat
	{
		Span span;
		using KindUnion = std::variant<PatKind::Wild, PatKind::Ident, PatKind::Or, PatKind::Ref, PatKind::Path,
			PatKind::Paren,PatKind::Literal>;
		KindUnion kind;
		NodeId id;

		Pat(Span span, KindUnion&& kind, NodeId id = DUMMY_NODE_ID) : span{span}, kind{kind}, id{id}
		{}
	};



}

#endif //CORROSION_SRC_AST_NODE_PATTERN_HPP_
