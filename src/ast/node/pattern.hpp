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
	struct PatternKind
	{
		/// Represents a wildcard pattern (`_`).
		struct Wild
		{
		};
		/// A `PatKind::Ident` may either be a new bound variable (`ref mut binding @ OPT_SUBPATTERN`),
		/// or a unit struct/variant pattern, or a const pattern (in the last two cases the third
		/// field must be `None`). Disambiguation cannot be done with parser alone, so it happens
		/// during name resolution.
		struct PIdent
		{
			BindingMode bindingMode;
			Ident ident;
			std::optional<Pointer<Pattern>> pat;
		};
		/// An or-pattern `A | B | C`.
		/// Invariant: `pats.len() >= 2`.
		struct Or
		{
			std::vector<Pointer<Pattern>> pats;
		};

		struct Path
		{

		};
	};
	struct Pattern
	{
		NodeId id;
		PatternKind kind;
		Span span;
	};



}

#endif //CORROSION_SRC_AST_NODE_PATTERN_HPP_
