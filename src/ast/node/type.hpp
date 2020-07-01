#ifndef CORROSION_SRC_AST_NODE_TYPE_HPP_
#define CORROSION_SRC_AST_NODE_TYPE_HPP_

#include "ast/fwd.hpp"

#include "ast/node/expr.hpp"
#include "ast/node/item.hpp"

namespace corrosion
{
	struct BareFnType
	{
		Pointer<FnDecl> decl;
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
			//std::optional<QPath> qpath;
			corrosion::Path path;
		};
		/// A bare function (e.g., `fn(usize) -> bool`).
		struct BareFn
		{
			Pointer<BareFnType> ptr;
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
		Span span;
		using KindUnion = std::variant<TyKind::Array,TyKind::Path, TyKind::BareFn, TyKind::Slice>;
		KindUnion kind;

		NodeId id;

		Ty(const Span& span, KindUnion&& kind, NodeId id = DUMMY_NODE_ID) : span{span},kind{kind},id{id}
		{}
	};
}

#endif //CORROSION_SRC_AST_NODE_TYPE_HPP_
