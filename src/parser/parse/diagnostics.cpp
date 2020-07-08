#include "parser/parser.hpp"

namespace corrosion
{
	/// Produces an error if comparison operators are chained (RFC #558).
	/// We only need to check the LHS, not the RHS, because all comparison ops have same
	/// precedence (see `fn precedence`) and are left-associative (see `fn fixity`).
	///
	/// This can also be hit if someone incorrectly writes `foo<bar>()` when they should have used
	/// the turbofish (`foo::<bar>()`) syntax. We attempt some heuristic recovery if that is the
	/// case.
	///
	/// Keep in mind that given that `outer_op.is_comparison()` holds and comparison ops are left
	/// associative we can infer that we have:
	///
	///           outer_op
	///           /   \
    ///     inner_op   r2
	///        /  \
    ///      l1    r1
	bool Parser::checkNoChainedComparison(Pointer<Expr> innerOp, Spanned<AssocOp>&& outerOp)
	{
		assert(outerOp.first.isComparison());
		if(std::holds_alternative<ExprKind::Binary>(innerOp->kind))
		{
			auto op =std::get<ExprKind::Binary>(innerOp->kind).oper;
			if(BinOp::isComprasion(op))
			{
				session->errorSpan(outerOp.second, "comparison operators cannot be chained");
				return true;
			}
		}
		return false;
	}

}