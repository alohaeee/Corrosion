#ifndef CORROSION_SRC_PARSER_AST_AST_FWD_HPP_
#define CORROSION_SRC_PARSER_AST_AST_FWD_HPP_

#include "utility/std_incl.hpp"
#include "utility/variant_wrapper.hpp"
#include "span/span.hpp"
#include "span/symbol.hpp"
#include "token.hpp"

namespace corrosion
{
	template<typename T>
	using Pointer = std::shared_ptr<T>;

	template<class Ty, class... Types>
	inline Pointer<Ty> MakePointer(Types&& ... args)
	{
		return std::make_shared<Ty>(std::forward<Types>(args)...);
	}

	using NodeId = std::uint32_t;

	enum : NodeId
	{
		DUMMY_NODE_ID = std::numeric_limits<NodeId>::max()
	};

	// >------Item ------<
	/// A "Label" is an identifier of some point in sources,
	/// e.g. in the following code:
	///
	/// ```rust
	/// 'outer: loop {
	///     break 'outer;
	/// }
	/// ```
	///
	/// `'outer` is a label.
	struct Label;

	/// A "Lifetime" is an annotation of the scope in which variable
	/// can be used, e.g. `'a` in `&'a i32`.
	struct Lifetime;

	/// A segment of a path: an identifier, an optional lifetime, and a set of types.
	///
	/// E.g., `std`, `String` or `Box<T>`.
	struct PathSegmenth;

	/// A "Path" is essentially Rust's notion of a name.
	///
	/// It's represented as a sequence of identifiers,
	/// along with a bunch of supporting information.
	///
	/// E.g., `std::cmp::PartialEq`.
	struct Path;

	/// A block (`{ .. }`).
	///
	/// E.g., `{ .. }` as in `fn foo() { .. }`.
	struct Block;

	// >------Pattern------<
	enum class Mutability;
	struct BindingMode;
	struct PatternKind;
	/// The kind of borrow in an `AddrOf` expression,
	/// e.g., `&place` or `&raw const place`.
	enum class BorrowKind;
	struct Pattern;
	// >------Op------<
	enum class BinOpKind;
	struct BinOp;
	enum class UnOp;

	// >------Literal------<
	struct Literal;

	// >------Types------<
	struct TyKind;
	struct Ty;

	// >------Expr------<

	enum class RangeLimits;

	/// The explicit `Self` type in a "qualified path". The actual
	/// path, including the trait and the associated item, is stored
	/// separately. `position` represents the index of the associated
	/// item qualified with this `Self` type.
	///
	/// ```ignore (only-for-syntax-highlight)
	/// <Vec<T> as a::b::Trait>::AssociatedItem
	///  ^~~~~     ~~~~~~~~~~~~~~^
	///  ty        position = 3
	///
	/// <Vec<T>>::AssociatedItem
	///  ^~~~~    ^
	///  ty       position = 0
	///	 ```
	struct QSelf;
	/// An expression.
	struct Expr;

	/// A constant (expression) that's not an item or associated item,
	/// but needs its own `DefId` for type-checking, const-eval, etc.
	/// These are usually found nested inside types (e.g., array lengths)
	/// or expressions (e.g., repeat counts), and also used to define
	/// explicit discriminant values for enum variants.
	struct AnonConst;

	// >------Stmt------<
	struct Stmt;

}

#endif //CORROSION_SRC_PARSER_AST_AST_FWD_HPP_
