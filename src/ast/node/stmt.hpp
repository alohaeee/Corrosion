#ifndef CORROSION_SRC_AST_NODE_STMT_HPP_
#define CORROSION_SRC_AST_NODE_STMT_HPP_

#include "ast/fwd.hpp"

namespace corrosion
{
	/// Local represents a `let` statement, e.g., `let <pat>:<ty> = <expr>;`.
	struct Local
	{
		Span span;
		Pointer<Pat> pat;
		Pointer<Ty> type;
		/// Initializer expression to set the value, if any.
		Pointer<Expr> init;
		NodeId id;
		Local(const Span& span, Pointer<Pat>& pat, Pointer<Ty>& type, Pointer<Expr>& init, NodeId id = DUMMY_NODE_ID) :
			span{ span }, pat{ pat }, type{ type }, init{ init }, id{ id }
		{

		}
		void printer(std::size_t level)
		{
			astLogPrint("let <pat>:<ty> = <expr>;", level);
			astLogPrint("pat:", level);
			if (pat)
			{
				pat->printer(level + 1);
			}
			else
			{
				astLogPrint("BUG: There must be pattern", level + 1);
			}
			if (type)
			{
				astLogPrint("type:", level);
				type->printer(level + 1);
			}
			if (init)
			{
				astLogPrint("init:", level);
				init->printer(level + 1);
			}
		}
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
		using KindUnion = std::variant<StmtKind::Local,
									   StmtKind::Item,
									   StmtKind::Expr,
									   StmtKind::Semi,
									   StmtKind::Empty>;
		Span span;
		KindUnion kind;
		NodeId id;

		Stmt(const Span& span, KindUnion&& kind, NodeId id = DUMMY_NODE_ID) : span(span), kind(kind), id(id)
		{
		}

		void printer(std::size_t level)
		{
			auto label = nodeFormatter("Stmt", id, span);
			astLogPrint(label, level);
			std::visit([level](auto&& arg)
			{
			  using T = std::decay_t<decltype(arg)>;
			  if constexpr(std::is_same_v<T, StmtKind::Local>)
			  {
				  astLogPrint("kind: Local", level + 1);
				  astLogPrint("internal:", level + 1);
				  arg.local->printer(level + 2);
			  }
			  else
			  {
				  astLogPrint("BUG: Try to print Stmt as AST Node but fell through all visit cases", level + 1);
			  }
			}, kind);
		}
	};
}

#endif //CORROSION_SRC_AST_NODE_STMT_HPP_
