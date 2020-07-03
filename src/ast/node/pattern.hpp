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
		} kind;
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
			Pointer<Pat> subpat;
		};
		/// An or-pattern `A | B | C`.
		/// Invariant: `pats.len() >= 2`.
		struct Or
		{
			std::vector<Pointer < Pat>> pats;
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
									   PatKind::Paren, PatKind::Literal>;
		KindUnion kind;
		NodeId id;

		Pat(Span span, KindUnion&& kind, NodeId id = DUMMY_NODE_ID) : span{ span }, kind{ kind }, id{ id }
		{
		}

		void printer(std::size_t level)
		{
			auto label = nodeFormatter("Pattern", id, span);
			astLogPrint(label, level);
			std::visit([level](auto&& arg)
			{
			  using T = std::decay_t<decltype(arg)>;
			  if constexpr(std::is_same_v<T, PatKind::Wild>)
			  {
				  astLogPrint("type: Wild(_)", level + 1);
			  }
			  else if constexpr(std::is_same_v<T, PatKind::Ident>)
			  {
				  astLogPrint("type: Ident", level + 1);
				  if (arg.bindingMode.kind == BindingMode::ByRef)
				  {
					  astLogPrint("binding: By Ref", level + 1);
				  }
				  if (arg.bindingMode.mut == Mutability::Mut)
				  {
					  astLogPrint("mutability: mut", level + 1);
				  }
				  else
				  {
					  astLogPrint("mutability: not", level + 1);
				  }

				  astLogPrint(fmt::format("name: {}", arg.ident.name().toString()), level + 1);
				  if (arg.subpat)
				  {
					  astLogPrint("subpat:", level + 1);
					  arg.subpat->printer(level + 2);
				  }

			  }
			  else
			  {
				  astLogPrint("BUG: Fell through of PatKind", level + 1);
			  }
			}, kind);
		}
	};

}

#endif //CORROSION_SRC_AST_NODE_PATTERN_HPP_
