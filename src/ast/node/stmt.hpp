#ifndef CORROSION_SRC_AST_NODE_STMT_HPP_
#define CORROSION_SRC_AST_NODE_STMT_HPP_

#include "ast/fwd.hpp"

namespace corrosion
{
	/// Local represents a `let` statement, e.g., `let <pat>:<ty> = <expr>;`.
	struct Local
	{
		NodeId id;
		Pointer<Pat> pat;
		std::optional<Pointer<Ty>> type;
		/// Initializer expression to set the value, if any.
		std::optional<Pointer<Expr>> expr;
		Span span;
	};

	struct StmtKind
	{
		struct Local
		{
			Pointer<corrosion::Local> local;
		};
		struct Item
		{
			Pointer<corrosion::Item> item;
		};
		struct Expr
		{
			Pointer<corrosion::Expr> expr;
		};
		struct Semi
		{
			Pointer<corrosion::Expr> expr;
		};
		struct Empty
		{

		};
	};
	struct Stmt
	{

		using KindUnion = std::variant<StmtKind::Local,StmtKind::Item,StmtKind::Expr,StmtKind::Semi,StmtKind::Empty>;
		NodeId id;
		StmtKind kind;
		Span span;
	};
}

#endif //CORROSION_SRC_AST_NODE_STMT_HPP_
