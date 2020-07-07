#include "expr.hpp"
#include "stmt.hpp"

namespace corrosion
{

	bool Expr::requiresSemiToBeStmt()
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
	bool Expr::returns() const noexcept
	{
		if (std::holds_alternative<ExprKind::Block>(kind))
		{
			auto block = std::get<ExprKind::Block>(kind).block;
			std::visit([](auto&& arg) -> bool
			{
			  using T = std::decay_t<decltype(arg)>;

			  if constexpr(std::is_same_v<T, StmtKind::Expr>)
			  {
				  // Implicit return
				  return true;
			  }
			  else if constexpr(std::is_same_v<T, StmtKind::Semi>)
			  {
				  if (std::holds_alternative<ExprKind::Return>(arg.expr->kind))
				  {
					  // Last statement is explicit return.
					  return true;
				  }
				  return false;
			  }
			  else
			  {
				  // This is a block that doesn't end in either an implicit or explicit return.
				  return false;
			  }
			}, block->stmts.back().kind);
		}
		else
		{
			// This is not a block, it is a value.
			return true;
		}
	}
	void Expr::printer(std::size_t level)
	{
		auto label = nodeFormatter("Expr", id, span);
		astLogPrint(label, level);
		std::visit([this, level](auto&& arg)
		{
		  using T = std::decay_t<decltype(arg)>;

		  if constexpr(std::is_same_v<T, ExprKind::If>)
		  {
			  astLogPrint("type: If", level + 1);
			  astLogPrint("cond:", level + 1);
			  if (arg.condition)
			  {
				  arg.condition->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be condition expr", level + 2);
			  }
			  astLogPrint("block:", level + 1);
			  if (arg.block)
			  {
				  arg.block->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be block expr", level + 2);
			  }
			  if (arg.elseExpr)
			  {
				  astLogPrint("else:", level + 1);
				  arg.elseExpr->printer(level + 2);
			  }

		  }
		  else if constexpr(std::is_same_v<T, ExprKind::Match>)
		  {
			  astLogPrint("type: Match", level + 1);
		  }
		  else if constexpr (std::is_same_v<T, ExprKind::While>)
		  {
			  astLogPrint("type: While", level + 1);
			  if (arg.label)
			  {
				  astLogPrint(fmt::format("label: {}", arg.label->ident.name().toString()), level + 1);
			  }
			  astLogPrint("cond:", level + 1);
			  if (arg.condition)
			  {
				  arg.condition->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be condition expr", level + 2);
			  }
			  if (arg.block)
			  {
				  arg.block->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be block expr", level + 2);
			  }
		  }
		  else if constexpr (std::is_same_v<T, ExprKind::Loop>)
		  {
			  astLogPrint("type: Loop", level + 1);
			  if (arg.label)
			  {
				  astLogPrint(fmt::format("label: {}", arg.label->ident.name().toString()), level + 1);
			  }
			  if (arg.block)
			  {
				  arg.block->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be block expr", level + 2);
			  }

		  }
		  else if constexpr (std::is_same_v<T, ExprKind::ForLoop>)
		  {
			  astLogPrint("type: ForLoop", level + 1);
			  if (arg.label)
			  {
				  astLogPrint(fmt::format("label: {}", arg.label->ident.name().toString()), level + 1);
			  }
			  if (arg.pat)
			  {
				  astLogPrint("pat:", level + 1);
				  arg.pat->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be pattern", level + 2);
			  }
			  if (arg.expr)
			  {
				  astLogPrint("expr:", level + 1);
				  arg.expr->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be expr", level + 2);
			  }
			  if (arg.block)
			  {
				  arg.block->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be block expr", level + 2);
			  }

		  }
		  else if constexpr (std::is_same_v<T, ExprKind::Literal>)
		  {
			  astLogPrint("type: Literal", level + 1);
			  astLogPrint("internal:", level + 1);
			  arg.printer(level + 2);
		  }
		  else if constexpr(std::is_same_v<T, ExprKind::Binary>)
		  {
			  astLogPrint("type: Binary", level + 1);
			  astLogPrint(fmt::format("op: {}", BinOp::toString(arg.oper)), level + 1);
			  if (arg.lhs)
			  {
				  astLogPrint("lhs:", level + 1);
				  arg.lhs->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be lhs arg", level + 2);
			  }
			  if (arg.rhs)
			  {
				  astLogPrint("rhs:", level + 1);
				  arg.rhs->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be rhs arg", level + 2);
			  }
		  }
		  else if constexpr(std::is_same_v<T, ExprKind::Unary>)
		  {
			  astLogPrint("type: Unary", level + 1);
			  astLogPrint(fmt::format("op: {}", UnOp::toString(arg.oper)), level + 1);
			  if (arg.expr)
			  {
				  arg.expr->printer(level + 1);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be expr", level + 2);
			  }
		  }
		  else if constexpr (std::is_same_v<T, ExprKind::Return>)
		  {
			  astLogPrint("type: Return", level + 1);
			  if (arg.expr)
			  {
				  astLogPrint("expr:", level + 1);
				  arg.expr->printer(level + 2);
			  }
		  }
		  else if constexpr (std::is_same_v<T, ExprKind::Continue>)
		  {
			  astLogPrint("type: Continue", level + 1);
			  if (arg.label)
			  {
				  astLogPrint(fmt::format("label: {}", arg.label->ident.name().toString()), level + 1);
			  }
		  }
		  else if constexpr (std::is_same_v<T, ExprKind::Break>)
		  {
			  astLogPrint("type: Break", level + 1);
			  if (arg.label)
			  {
				  astLogPrint(fmt::format("label: {}", arg.label->ident.name().toString()), level + 1);
			  }
			  if (arg.expr)
			  {
				  astLogPrint("expr:", level + 1);
				  arg.expr->printer(level + 2);
			  }
		  }
		  else if constexpr (std::is_same_v<T, ExprKind::Paren>)
		  {
			  astLogPrint("type: Paren", level + 1);
			  if (arg.expr)
			  {
				  arg.expr->printer(level + 1);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be internal expr", level + 1);
			  }
		  }
		  else if constexpr (std::is_same_v<T, ExprKind::Block>)
		  {
			  astLogPrint("type: Block", level + 1);
			  if (arg.label)
			  {
			  	astLogPrint(fmt::format("label: {}", arg.label->ident.name().toString()),level+1);
			  }
			  if(arg.block)
			  {
			  	arg.block->printer(level+1);
			  }
			  else
			  {
				  astLogPrint("BUG: There must be internal expr", level + 1);
			  }
		  }
		  else if constexpr (std::is_same_v<T, ExprKind::Error>)
		  {
			  astLogPrint("type: Error", level + 1);
		  }
		  else
		  {
			  astLogPrint("BUG: Try to print Expr as AST Node but fell through all visit cases", level + 1);
		  }

		}, kind);
	}

}