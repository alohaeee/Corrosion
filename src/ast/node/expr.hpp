#ifndef CORROSION_SRC_AST_NODE_EXPR_HPP_
#define CORROSION_SRC_AST_NODE_EXPR_HPP_

#include "ast/fwd.hpp"

#include "ast/node/item.hpp"
#include "ast/node/op.hpp"
#include "ast/node/literal.hpp"

namespace corrosion
{
	/// An arm of a 'match'.
	///
	/// E.g., `0..=10 => { println!("match!") }` as in
	///
	/// ```
	/// match 123 {
	///     0..=10 => { println!("match!") },
	///     _ => { println!("no match!") },
	/// }
	/// ```
	struct Arm
	{
		/// Match arm pattern, e.g. `10` in `match foo { 10 => {}, _ => {} }`
		Pointer<Pat> pat;
		/// Match arm guard, e.g. `n > 10` in `match foo { n if n > 10 => {}, _ => {} }`
		std::optional<Pointer<Expr>> guard;
		/// Match arm body.
		Pointer<Expr> body;
		NodeId id;
		bool isPlaceHolder;

	};
	struct AnonConst
	{
		NodeId id;
		Pointer<Expr> value;
	};
	enum class RangeLimits
	{
		/// Inclusive at the beginning, exclusive at the end
		HalfOpen,
		/// Inclusive at the beginning and end
		Closed
	};
	struct QSelf
	{
		Pointer<Ty> type;
		/// The span of `a::b::Trait` in a path like `<Vec<T> as
		/// a::b::Trait>::AssociatedItem`; in the case where `position ==
		/// 0`, this is an empty span.
		Span path;
		std::size_t position;

	};

	struct ExprKind
	{
		/// An array (`[a, b, c, d]`)
		struct Array
		{
			std::vector<Pointer<Expr>> exprs;
		};
		/// A function call
		///
		/// The first field resolves to the function itself,
		/// and the second field is the list of arguments.
		/// This also represents calling the constructor of
		/// tuple-like ADTs such as tuple structs and enum variants.
		struct FunctionCall
		{
			Pointer<Expr> func;
			std::vector<Pointer<Expr>> args;
		};

		/// A binary operation (e.g., `a + b`, `a * b`).
		struct Binary
		{
			BinOpKind oper;
			Pointer<Expr> lhs;
			Pointer<Expr> rhs;
		};
		/// A unary operation (e.g., `!x`, `*x`).
		struct Unary
		{
			UnOp oper;
			Pointer<Expr> expr;
		};
		/// A cast (e.g., `foo as f64`).
		struct Cast
		{
			Pointer<Expr> expr;
			Pointer<Ty> type;
		};
		/// A type ascription (e.g., `42: usize`).
		struct Type
		{
			Pointer<Expr> expr;
			Pointer<Ty> type;
		};
		/// A literal (e.g., `1`, `"foo"`).
		using Literal = Literal;
		/// A `let pat = expr` expression that is only semantically allowed in the condition
		/// of `if` / `while` expressions. (e.g., `if let 0 = x { .. }`).
		struct Let
		{
			Pointer<Pat> pat;
			Pointer<Expr> expr;
		};
		/// An `if` block, with an optional `else` block.
		///
		/// `if expr { block } else { expr }`
		struct If
		{
			Pointer<Expr> condition;
			Pointer<Block> block;
			Pointer<Expr> elseExpr;
		};

		/// A while loop, wi
		/// th an optional label.
		///
		/// `'label: while expr { block }`
		struct While
		{
			Pointer<Expr> condition;
			Pointer<Block> block;
			std::optional<Label> label;
		};

		/// A `for` loop, with an optional label.
		///
		/// `'label: for pat in expr { block }`
		///
		/// This is desugared to a combination of `loop` and `match` expressions.
		struct ForLoop
		{
			Pointer<Pat> pat;
			Pointer<Expr> expr;
			Pointer<Block> block;
			std::optional<Label> label;
		};

		/// Conditionless loop (can be exited with `break`, `continue`, or `return`).
		///
		/// `'label: loop { block }`
		struct Loop
		{
			Pointer<Block> block;
			std::optional<Label> label;
		};
		/// A `match` block.
		struct Match
		{
			Pointer<Expr> expr;
			std::vector<Arm> arms;
		};

		struct Block
		{
			Pointer<corrosion::Block> block;
			std::optional<Label> label;
		};
		/// An assignment (`a = foo()`).
		/// The `Span` argument is the span of the `=` token.
		struct Assign
		{
			Pointer<Expr> lhs;
			Pointer<Expr> rhs;
			Span eq;
		};
		/// An assignment with an operator.
		///
		/// E.g., `a += 1`.
		struct AssignOp
		{
			BinOpKind op;
			Pointer<Expr> lhs;
			Pointer<Expr> rhs;
		};
		/// An indexing operation (e.g., `foo[2]`).
		struct Index
		{
			Pointer<Expr> e;
			Pointer<Expr> e1;
		};
		/// A range (e.g., `1..2`, `1..`, `..2`, `1..=2`, `..=2`).
		struct Range
		{
			Pointer<Expr> lhs;
			Pointer<Expr> rhs;
			RangeLimits limits;
		};
		/// Variable reference, possibly containing `::` and/or type
		/// parameters (e.g., `foo::bar::<baz>`).
		///
		/// Optionally "qualified" (e.g., `<Vec<T> as SomeTrait>::SomeType`).
		struct Path
		{
			std::optional<QSelf> qSelf;
			corrosion::Path path;
		};
		/// A referencing operation (`&a`, `&mut a`, `&raw const a` or `&raw mut a`).
		struct AddrOf
		{
			BorrowKind borrow;
			Mutability mut;
			Pointer<Expr> expr;
		};
		/// A `break`, with an optional label to break, and an optional expression.
		struct Break
		{
			std::optional<Label> label;
			Pointer<Expr> expr;
		};
		/// A `continue`, with an optional label.
		struct Continue
		{
			std::optional<Label> label;
		};
		/// A `return`, with an optional value to be returned.
		struct Return
		{
			Pointer<Expr> expr;
		};
		/// A try expression (`expr?`).
		struct Try
		{
			Pointer<Expr> expr;
		};
		struct Error
		{

		};
	};

	struct Expr
	{
		Span span;
		using KindUnion = std::variant<ExprKind::Array,
									   ExprKind::FunctionCall,
									   ExprKind::Binary,
									   ExprKind::Unary,
									   ExprKind::Cast,
									   ExprKind::Literal,
									   ExprKind::Let,
									   ExprKind::If,
									   ExprKind::While,
									   ExprKind::ForLoop,
									   ExprKind::Loop,
									   ExprKind::Match,
									   ExprKind::Block,
									   ExprKind::Assign,
									   ExprKind::AssignOp,
									   ExprKind::Index,
									   ExprKind::Range,
									   ExprKind::Path,
									   ExprKind::AddrOf,
									   ExprKind::Break,
									   ExprKind::Continue,
									   ExprKind::Try,
									   ExprKind::Return>;
		KindUnion kind{};
		NodeId id;

		Expr(const Span& span, KindUnion&& kind, NodeId id = DUMMY_NODE_ID) : span(span), kind(kind), id(id)
		{
		}
		Expr(Span&& span, KindUnion&& kind, NodeId id = DUMMY_NODE_ID) : span(span), kind(kind), id(id)
		{
		}
		/// Does this expression require a semicolon to be treated
		/// as a statement? The negation of this: 'can this expression
		/// be used as a statement without a semicolon' -- is used
		/// as an early-bail-out in the parser so that, for instance,
		///     if true {...} else {...}
		///      |x| 5
		/// isn't parsed as (if true {...} else {...} | x) | 5
		bool requiresSemiToBeStmt()
		{
			return std::visit([](auto&& arg) -> bool
			{
			  using T = std::decay_t<decltype(arg)>;
			  if constexpr(std::is_same_v<T, ExprKind::If>)
			  {
			  }
			  else if constexpr(std::is_same_v<T, ExprKind::Match>)
			  {
			  }
			  else if constexpr (std::is_same_v<T, ExprKind::While>)
			  {
			  }
			  else if constexpr (std::is_same_v<T, ExprKind::Loop>)
			  {
			  }
			  else if constexpr (std::is_same_v<T, ExprKind::ForLoop>)
			  {
			  }
			  else
			  {
				  return false;
			  }
			  return true;

			}, kind);
		}
		void printer(std::size_t level)
		{
			auto label = nodeFormatter("Expr", id, span);
			astLogPrint(label,level);
			std::visit([this,level](auto&& arg)
			{
			  using T = std::decay_t<decltype(arg)>;

			  if constexpr(std::is_same_v<T, ExprKind::If>)
			  {
			  	astLogPrint("type: If",level+1);
				astLogPrint("cond:", level+1);
				if(arg.condition)
				{
					arg.condition->printer(level+2);
				}
				else
				{
					astLogPrint("BUG: There must be condition expr", level+2);
				}
				astLogPrint("block:", level+1);
				if(arg.block)
				{
					//	arg.block->printer(level+2);
				}
				else
				{
					astLogPrint("BUG: There must be block expr", level+2);
				}
				if(arg.elseExpr)
				{
					astLogPrint("else:", level+1);
					arg.elseExpr->printer(level+2);
				}

			  }
			  else if constexpr(std::is_same_v<T, ExprKind::Match>)
			  {
				  astLogPrint("type: Match",level+1);
			  }
			  else if constexpr (std::is_same_v<T, ExprKind::While>)
			  {
				  astLogPrint("type: While",level+1);
				  if(arg.label)
				  {
						astLogPrint(fmt::format("Label: {}", arg.label->ident.name().toString()), level +1);
				  }
				  astLogPrint("cond:", level+1);
				  if(arg.condition)
				  {
					  arg.condition->printer(level+2);
				  }
				  else
				  {
					  astLogPrint("BUG: There must be condition expr", level+2);
				  }
				  if(arg.block)
				  {
					  //	arg.block->printer(level+2);
				  }
				  else
				  {
					  astLogPrint("BUG: There must be block expr", level+2);
				  }
			  }
			  else if constexpr (std::is_same_v<T, ExprKind::Loop>)
			  {
				  astLogPrint("type: Loop",level+1);
				  if(arg.label)
				  {
					  astLogPrint(fmt::format("Label: {}", arg.label->ident.name().toString()), level +1);
				  }
				  if(arg.block)
				  {
					  //	arg.block->printer(level+2);
				  }
				  else
				  {
					  astLogPrint("BUG: There must be block expr", level+2);
				  }

			  }
			  else if constexpr (std::is_same_v<T, ExprKind::ForLoop>)
			  {
				  astLogPrint("type: ForLoop",level+1);
				  if(arg.label)
				  {
					  astLogPrint(fmt::format("Label: {}", arg.label->ident.name().toString()), level +1);
				  }
				  if(arg.pat)
				  {
					  astLogPrint("Pat:", level+1);
					  arg.pat->printer(level+2);
				  }
				  if(arg.block)
				  {
					  //	arg.block->printer(level+2);
				  }

			  }
			  else if constexpr (std::is_same_v<T, ExprKind::Literal>)
			  {
				  astLogPrint("type: Literal",level+1);
				  astLogPrint("internal:", level+1);
				  arg.printer(level+2);
			  }
			  else
			  {
				  astLogPrint("BUG: Try to print Expr as AST Node but fell through all visit cases", level+1);
			  }
//			  if(type.empty())
//			  {
//			  	type = "BUG: some expression type was't visit";
//			  }
//			  else
//			  {
//			  	type = fmt::format("type: {}", type);
//			  }
//			  astLogPrint(type,level+1);

			}, kind);
		}
	};
}

#endif //CORROSION_SRC_AST_NODE_EXPR_HPP_
