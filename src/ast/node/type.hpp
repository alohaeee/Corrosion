#ifndef CORROSION_SRC_AST_NODE_TYPE_HPP_
#define CORROSION_SRC_AST_NODE_TYPE_HPP_

#include "ast/fwd.hpp"

#include "ast/node/expr.hpp"
#include "ast/node/item.hpp"

namespace corrosion
{
	struct BareFnType
	{

	};

	struct MutType
	{
		Pointer<Ty> type;
		Mutability mut;
	};

	struct TyKind
	{
		/// A variable-length slice (`[T]`).
		struct Slice
		{
			Pointer<Ty> type;
		};
		/// A fixed length array (`[T; n]`).
		struct Array
		{
			Pointer<Ty> type;
			AnonConst len;
		};
		/// A raw pointer (`*const T` or `*mut T`).
		struct Ptr
		{
			MutType mut;
		};
		/// A reference (`&'a T` or `&'a mut T`).
		struct Rptr
		{
			std::optional<Lifetime> lifetime;
			MutType mut;
		};
		/// A tuple (`(A, B, C, D,...)`).
		struct Tuple
		{
			std::vector<Pointer<Ty>> tup;
		};
		/// Type parameters are stored in the `Path` itself.
		struct Path
		{

		};
		/// A bare function (e.g., `fn(usize) -> bool`).
		struct BareFn
		{
		};
		/// Inferred type of a `self` or `&self` argument in a method.
		struct ImplicitSelf
		{
		};
		struct Err
		{

		};
	};
	struct Ty
	{
		NodeId id;
		TyKind kind;
		Span span;
	};
}

#endif //CORROSION_SRC_AST_NODE_TYPE_HPP_
