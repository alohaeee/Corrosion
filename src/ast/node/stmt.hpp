#ifndef CORROSION_SRC_AST_NODE_STMT_HPP_
#define CORROSION_SRC_AST_NODE_STMT_HPP_

#include "ast/fwd.hpp"

namespace corrosion
{
	struct Local
	{
		NodeId id;

	};
	struct Stmt
	{
		enum StmtKind
		{

		};
		NodeId id;
		StmtKind kind;
		Span span;
	};
}

#endif //CORROSION_SRC_AST_NODE_STMT_HPP_
