#ifndef CORROSION_SRC_AST_ASSOC_PREC_HPP_
#define CORROSION_SRC_AST_ASSOC_PREC_HPP_
#include "utility/std_incl.hpp"
#include "ast/token.hpp"
#include "ast/ast.hpp"

namespace corrosion
{
	enum Fixity
	{
		/// The operator is left-associative
		Left,
		/// The operator is right-associative
		Right,
		/// The operator is not associative
		None,
	};
	struct AssocOp
	{
		enum
		{
			/// `+`
			Add,
			/// `-`
			Subtract,
			/// `*`
			Multiply,
			/// `/`
			Divide,
			/// `%`
			Modulus,
			/// `&&`
			LAnd,
			/// `||`
			LOr,
			/// `^`
			BitXor,
			/// `&`
			BitAnd,
			/// `|`
			BitOr,
			/// `<<`
			ShiftLeft,
			/// `>>`
			ShiftRight,
			/// `==`
			Equal,
			/// `<`
			Less,
			/// `<=`
			LessEqual,
			/// `!=`
			NotEqual,
			/// `>`
			Greater,
			/// `>=`
			GreaterEqual,
			/// `=`
			Assign,
			/// `?=` where ? is one of the BinOpToken
			AssignOp,
			/// `as`
			As,
			/// `..` range
			DotDot,
			/// `..=` range
			DotDotEq,
			/// `:`
			Colon,
		} kind;
		static std::optional<AssocOp> fromToken(ast::Token tok)
		{
			switch (tok.kind)
			{
			case ast::TokenKind::BinOp:
			{
				auto data = tok.getData<ast::data::BinOp>();
				switch (data.kind)
				{
				case ast::data::BinOp::Star:
					return AssocOp{ AssocOp::Multiply };
				case ast::data::BinOp::Slash:
					return AssocOp{ AssocOp::Divide };
				case ast::data::BinOp::Percent:
					return AssocOp{ AssocOp::Modulus };
				case ast::data::BinOp::Plus:
					return AssocOp{ AssocOp::Add };
				case ast::data::BinOp::Minus:
					return AssocOp{ AssocOp::Subtract };
				case ast::data::BinOp::Shl:
					return AssocOp{ AssocOp::ShiftLeft };
				case ast::data::BinOp::Shr:
					return AssocOp{ AssocOp::ShiftRight };
				case ast::data::BinOp::And:
					return AssocOp{ AssocOp::BitAnd };
				case ast::data::BinOp::Or:
					return AssocOp{ AssocOp::BitOr };
				case ast::data::BinOp::Caret:
					return AssocOp{ AssocOp::BitXor };
				}
			}
			case ast::TokenKind::Lt:
				return AssocOp{ AssocOp::Less };
			case ast::TokenKind::Le:
				return AssocOp{ AssocOp::LessEqual };
			case ast::TokenKind::Ge:
				return AssocOp{ AssocOp::GreaterEqual };
			case ast::TokenKind::Gt:
				return AssocOp{ AssocOp::Greater };
			case ast::TokenKind::EqEq:
				return AssocOp{ AssocOp::Equal };
			case ast::TokenKind::Ne:
				return AssocOp{ AssocOp::NotEqual };
			case ast::TokenKind::AndAnd:
				return AssocOp{ AssocOp::LAnd };
			case ast::TokenKind::OrOr:
				return AssocOp{ AssocOp::LOr };
			case ast::TokenKind::DotDot:
				return AssocOp{ AssocOp::DotDot };
			case ast::TokenKind::DotDotEq:
				return AssocOp{ AssocOp::DotDotEq };
			case ast::TokenKind::Colon:
				return AssocOp{ AssocOp::Colon };
			default:
				if (tok.isKeyword(kw::As))
				{
					return AssocOp{ AssocOp::As };
				}
				return std::nullopt;
			}
		}
		static AssocOp fromAstBinOp(BinOpKind op)
		{
			switch (op)
			{
			case BinOpKind::Lt:
				return { Less };
			case BinOpKind::Gt:
				return { Greater };
			case BinOpKind::Le:
				return { LessEqual };
			case BinOpKind::Ge:
				return { GreaterEqual };
			case BinOpKind::Eq:
				return { Equal };
			case BinOpKind::Ne:
				return { NotEqual };
			case BinOpKind::Mul:
				return { Multiply };
			case BinOpKind::Div:
				return { Divide };
			case BinOpKind::Rem:
				return { Modulus };
			case BinOpKind::Add:
				return { Add };
			case BinOpKind::Sub:
				return { Subtract };
			case BinOpKind::Shl:
				return { ShiftLeft };
			case BinOpKind::Shr:
				return { ShiftRight };
			case BinOpKind::BitAnd:
				return { BitAnd };
			case BinOpKind::BitXor:
				return { BitXor };
			case BinOpKind::BitOr :
				return { BitOr };
			case BinOpKind::And:
				return { LAnd };
			case BinOpKind::Or:
				return { LOr };
			}
		}
		std::optional<BinOpKind> toAstBinOp()
		{
			switch (kind)
			{
			case Less:
				return { BinOpKind::Lt };
			case Greater:
				return { BinOpKind::Gt };
			case LessEqual:
				return { BinOpKind::Le };
			case GreaterEqual:
				return { BinOpKind::Ge };
			case Equal:
				return { BinOpKind::Eq };
			case NotEqual:
				return { BinOpKind::Ne };
			case Multiply:
				return { BinOpKind::Mul };
			case Divide:
				return { BinOpKind::Div };
			case Modulus:
				return { BinOpKind::Rem };
			case Add:
				return { BinOpKind::Add };
			case Subtract:
				return { BinOpKind::Sub };
			case ShiftLeft:
				return { BinOpKind::Shl };
			case ShiftRight:
				return { BinOpKind::Shr };
			case BitAnd:
				return { BinOpKind::And };
			case BitXor:
				return { BinOpKind::BitXor };
			case BitOr:
				return { BinOpKind::BitOr };
			case LAnd:
				return { BinOpKind::And };
			case LOr:
				return { BinOpKind::Or };
			default:
				return std::nullopt;
			}
		}
		std::size_t precedence()
		{
			switch (kind)
			{
			case As:
			case Colon:
				return 14;
			case Multiply:
			case Divide:
			case Modulus:
				return 13;
			case Add:
			case Subtract:
				return 12;
			case ShiftLeft:
			case ShiftRight:
				return 11;
			case BitAnd:
				return 10;
			case BitXor:
				return 9;
			case Less:
			case Greater:
			case LessEqual:
			case GreaterEqual:
			case Equal:
			case NotEqual:
				return 7;
			case LAnd:
				return 6;
			case LOr:
				return 5;
			case DotDot:
			case DotDotEq:
				return 4;
			case Assign:
			case AssignOp:
				return 2;
			}
		}
		Fixity fixity()
		{
			switch (kind)
			{
			case As:
			case Colon:
			case Multiply:
			case Divide:
			case Modulus:
			case Add:
			case Subtract:
			case ShiftLeft:
			case ShiftRight:
			case BitAnd:
			case BitXor:
			case Less:
			case Greater:
			case LessEqual:
			case GreaterEqual:
			case Equal:
			case NotEqual:
			case LAnd:
			case LOr:
				return Fixity::Left;
			case DotDot:
			case DotDotEq:
				return Fixity::Right;
			case Assign:
			case AssignOp:
				return Fixity::None;
			}
		}
		bool isComparison() const noexcept
		{
			switch(kind)
			{
			case Greater:
			case GreaterEqual:
			case Less:
			case LessEqual:
			case Equal:
			case NotEqual:
				return true;
			default:
				return false;
			}
		}

		bool isAssignLike() const noexcept
		{
			switch(kind)
			{
			case Assign:
			case AssignOp:
				return true;
			default:
				return false;
			}
		}
		std::optional<ast::data::BinOp> optAssignOp{};
	};

	struct ExprPrecedence
	{
		static constexpr int8_t PREC_RESET = -100;
		static constexpr int8_t PREC_CLOSURE = -40;
		static constexpr int8_t PREC_JUMP = -30;
		static constexpr int8_t PREC_RANGE = -10;
		// The range 2..=14 is reserved for AssocOp binary operator precedences.
		static constexpr int8_t PREC_PREFIX = 50;
		static constexpr int8_t PREC_POSTFIX = 60;
		static constexpr int8_t PREC_PAREN = 99;
		static constexpr int8_t PREC_FORCE_PAREN = 100;
		enum
		{
			Closure,
			Break,
			Continue,
			Ret,
			Yield,

			Range,

			Binary,

			Cast,
			Type,

			Assign,
			AssignOp,

			Box,
			AddrOf,
			Let,
			Unary,

			Call,
			MethodCall,
			Field,
			Index,
			Try,
			InlineAsm,
			Mac,

			Array,
			Repeat,
			Tup,
			Lit,
			Path,
			Paren,
			If,
			While,
			ForLoop,
			Loop,
			Match,
			Block,
			TryBlock,
			Struct,
			Async,
			Await,
			Err,
		} kind;
		int8_t order()
		{
			switch (kind)
			{
			case Closure:
				return PREC_CLOSURE;
			case Break :
			case Continue :
			case Ret :
			case Yield:
				return PREC_JUMP;

				// `Range` claims to have higher precedence than `Assign`, but `x .. x = x` fails to
				// parse, instead of parsing as `(x .. x) = x`.  Giving `Range` a lower precedence
				// ensures that `pprust` will add parentheses in the right places to get the desired
				// parse.
			case Range:
				return PREC_RANGE;

				// Binop-like expr kinds, handled by `AssocOp`.
			case Binary:
				return static_cast<uint8_t>(AssocOp::fromAstBinOp(op.value()).precedence());

			case Cast :
				return static_cast<uint8_t>(AssocOp{ AssocOp::As }.precedence());

			case Type :
				return static_cast<uint8_t>(AssocOp{ AssocOp::Colon }.precedence());

			case Assign:
			case AssignOp:
				return static_cast<uint8_t>(AssocOp{ AssocOp::Assign }.precedence());
				// Unary, prefix
			case Box:
			case AddrOf:
				// Here `let pats = expr` has `let pats =` as a "unary" prefix of `expr`.
				// However, this is not exactly right. When `let _ = a` is the LHS of a binop we
				// need parens sometimes. E.g. we can print `(let _ = a) && b` as `let _ = a && b`
				// but we need to print `(let _ = a) < b` as-is with parens.
			case Let:
			case Unary:
				return PREC_PREFIX;

				// Unary, postfix
			case Await:
			case Call:
			case MethodCall:
			case Field:
			case Index:
			case Try:
			case InlineAsm:
			case Mac:
				return PREC_POSTFIX;

				// Never need parens
			case Array:
			case Repeat:
			case Tup:
			case Lit:
			case Path:
			case Paren:
			case If:
			case While:
			case ForLoop:
			case Loop:
			case Match:
			case Block:
			case TryBlock:
			case Async:
			case Struct:
			case Err:
				return PREC_PAREN;
			}
		}
		std::optional<BinOpKind> op;
	};
}

#endif //CORROSION_SRC_AST_ASSOC_PREC_HPP_
