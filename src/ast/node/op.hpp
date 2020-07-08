#ifndef CORROSION_SRC_AST_NODE_OP_HPP_
#define CORROSION_SRC_AST_NODE_OP_HPP_

#include "ast/fwd.hpp"
namespace corrosion
{
	enum class BinOpKind
	{
		/// The `+` operator (addition)
		Add,
		/// The `-` operator (subtraction)
		Sub,
		/// The `*` operator (multiplication)
		Mul,
		/// The `/` operator (division)
		Div,
		/// The `%` operator (modulus)
		Rem,
		/// The `&&` operator (logical and)
		And,
		/// The `||` operator (logical or)
		Or,
		/// The `^` operator (bitwise xor)
		BitXor,
		/// The `&` operator (bitwise and)
		BitAnd,
		/// The `|` operator (bitwise or)
		BitOr,
		/// The `<<` operator (shift left)
		Shl,
		/// The `>>` operator (shift right)
		Shr,
		/// The `==` operator (equality)
		Eq,
		/// The `<` operator (less than)
		Lt,
		/// The `<=` operator (less than or equal to)
		Le,
		/// The `!=` operator (not equal to)
		Ne,
		/// The `>=` operator (greater than or equal to)
		Ge,
		/// The `>` operator (greater than)
		Gt,
	};
	struct BinOp
	{
		static BinOpKind fromToken(ast::data::BinOp op)
		{
			switch (op.kind)
			{
			case ast::data::BinOp::Plus:
				return BinOpKind::Add;
			case ast::data::BinOp::Minus:
				return BinOpKind::Sub;
			case ast::data::BinOp::Star:
				return BinOpKind::Mul;
			case ast::data::BinOp::Slash:
				return BinOpKind::Div;
			case ast::data::BinOp::Percent:
				return BinOpKind::Rem;
			case ast::data::BinOp::And:
				return BinOpKind::BitAnd;
			case ast::data::BinOp::Or:
				return BinOpKind::BitOr;
			case ast::data::BinOp::Caret:
				return BinOpKind::BitXor;
			case ast::data::BinOp::Shl:
				return BinOpKind::Shl;
			case ast::data::BinOp::Shr:
				return BinOpKind::Shr;
			}
		}
		static std::string toString(BinOpKind op)
		{
			switch (op)
			{
			case BinOpKind::Add:
				return "+";
			case BinOpKind::Sub:
				return "-";
			case BinOpKind::Mul:
				return "*";
			case BinOpKind::Div:
				return "/";
			case BinOpKind::Rem:
				return "%";
			case BinOpKind::And:
				return "&&";
			case BinOpKind::Or:
				return "||";
			case BinOpKind::BitXor:
				return "^";
			case BinOpKind::BitAnd:
				return "&";
			case BinOpKind::BitOr:
				return "|";
			case BinOpKind::Shl:
				return "<<";
			case BinOpKind::Shr:
				return ">>";
			case BinOpKind::Eq:
				return "==";
			case BinOpKind::Lt:
				return "<";
			case BinOpKind::Le:
				return "<=";
			case BinOpKind::Ne:
				return "!=";
			case BinOpKind::Ge:
				return ">=";
			case BinOpKind::Gt:
				return ">";
			default:
				return "";
			}
		}
		static bool isComprasion(BinOpKind kind)
		{
			switch (kind)
			{
			case BinOpKind::Lt:
			case BinOpKind::Le:
			case BinOpKind::Gt:
			case BinOpKind::Ge:
			case BinOpKind::Ne:
			case BinOpKind::Eq:
				return true;
			default:
				return false;
			}
		}
	};

	enum class UnOpKind
	{
		/// The `*` operator for dereferencing
		Deref,
		/// The `!` operator for logical inversion
		Not,
		/// The `-` operator for negation
		Neg,
	};

	struct UnOp
	{
		static std::string toString(UnOpKind op)
		{
			switch (op)
			{
			case UnOpKind::Deref:
				return "*";
			case UnOpKind::Not:
				return "!";
			case UnOpKind::Neg:
				return "-";
			default:
				return "";
			}
		}
	};
}

#endif //CORROSION_SRC_AST_NODE_OP_HPP_
