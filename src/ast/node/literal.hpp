#ifndef CORROSION_SRC_AST_NODE_LITERAL_HPP_
#define CORROSION_SRC_AST_NODE_LITERAL_HPP_

#include "ast/fwd.hpp"

namespace corrosion
{

	class LiteralError : std::exception
	{
	 public:
		enum Kind
		{
			InvalidSuffix,
			NotImplemented
		};
		LiteralError(Kind errorKind) : m_errorKind{ errorKind }
		{}
		std::string toString() const noexcept
		{
			switch (m_errorKind)
			{
			case InvalidSuffix:
				return "Invalid suffix";
			case NotImplemented:
				return "Symbol was reserved, but for now is not implemented";
			default:
				return "";
			}
		}
	 private:
		Kind m_errorKind;

	};
	struct IntType
	{
		struct Signed
		{
			enum
			{
				ISize,
				I8,
				I16,
				I32,
				I64,
				I128,
			} type;
			std::string toString() const
			{
				switch (type)
				{
				case I8:
					return "i8";
				case I16:
					return "i16";
				case I32:
					return "i32";
				case I64:
					return "i64";
				case ISize:
					return "isize";
				case I128:
					return "i128";
				}
			}
			Symbol name() const
			{
				switch (type)
				{
				case I8:
					return sym::I8;
				case I16:
					return sym::I16;
				case I32:
					return sym::I32;
				case I64:
					return sym::I64;
				case ISize:
					return sym::ISize;
				case I128:
					return sym::I128;
				}
			}
		};
		struct Unsigned
		{
			enum
			{
				USize,
				U8,
				U16,
				U32,
				U64,
				U128,
			} type;
			std::string toString() const
			{
				switch (type)
				{
				case U8:
					return "u8";
				case U16:
					return "u16";
				case U32:
					return "u32";
				case U64:
					return "u64";
				case USize:
					return "usize";
				case U128:
					return "u128";
				}
			}
			Symbol name() const
			{
				switch (type)
				{
				case U8:
					return sym::U8;
				case U16:
					return sym::U16;
				case U32:
					return sym::U32;
				case U64:
					return sym::U64;
				case USize:
					return sym::USize;
				case U128:
					return sym::U128;
				}
			}
		};
		struct Unsuffixed
		{

		};
	};
	struct FloatType
	{
		struct Suffixed
		{
			enum
			{
				F32,
				F64
			} type;
			std::string toString() const
			{
				switch (type)
				{
				case F32:
					return "f32";
				case F64:
					return "f64";
				}
			}
			Symbol name() const
			{
				switch (type)
				{
				case F32:
					return sym::F32;
				case F64:
					return sym::F64;
				}
			}
		};
		struct Unsuffixed
		{

		};
	};
	struct LiteralKind
	{
		struct Str : Symbol
		{
			Symbol sym;
		};
		struct Char
		{
			char val;
		};
		struct Int
		{
			using KindUnion = std::variant<IntType::Signed, IntType::Unsigned, IntType::Unsuffixed>;
			std::uint64_t val;
			KindUnion type;
		};
		struct Float
		{
			using KindUnion = std::variant<FloatType::Suffixed, FloatType::Unsuffixed>;

			double val;
			KindUnion type;
		};
		struct Boolean
		{
			bool val;
		};
		struct Error
		{
			Symbol sym;
		};
	};
	struct Literal
	{
		/// The original literal token as written in source code.
		ast::data::Literal lit;
		Span span;
		/// The "semantic" representation of the literal lowered from the original tokens.
		/// Strings are unescaped, hexadecimal forms are eliminated, etc.
		using KindUnion = std::variant<LiteralKind::Char, LiteralKind::Float, LiteralKind::Int, LiteralKind::Str,
									   LiteralKind::Boolean, LiteralKind::Error>;
		KindUnion kind;

		inline bool isNumeric() const noexcept
		{
			return std::holds_alternative<LiteralKind::Float>(kind)
				|| std::holds_alternative<LiteralKind::Int>(kind);
		}
		inline bool isStr() const noexcept
		{
			return std::holds_alternative<LiteralKind::Str>(kind);
		}
		inline bool isSuffixed() const noexcept
		{

			if (auto ptr = std::get_if<LiteralKind::Int>(&kind); ptr)
			{
				return !std::holds_alternative<IntType::Unsuffixed>(ptr->type);
			}
			else if (auto ptr = std::get_if<LiteralKind::Float>(&kind); ptr)
			{
				return std::holds_alternative<FloatType::Suffixed>(ptr->type);
			}
			return false;
		}
		static Literal fromToken(const ast::Token& token)
		{
			assert(token.kind == ast::TokenKind::Literal);
			auto data = token.getData<ast::data::Literal>();
			auto kind = fromLitToken(data);
			return Literal{data,token.span,kind};
		}
		static Literal::KindUnion fromLitToken(const ast::data::Literal& lit)
		{
			switch (lit.kind)
			{
			case ast::data::Literal::Integer:
				return integerLiteral(lit.symbol, lit.suffix);
			case ast::data::Literal::Float:
				return filterFloatLiteral(lit.symbol,lit.suffix);
			case ast::data::Literal::Char:
				throw LiteralError{LiteralError::NotImplemented};
			case ast::data::Literal::Str:
				throw LiteralError{LiteralError::NotImplemented};
			case ast::data::Literal::Bool:
				assert(lit.symbol.isBoolLiteral());
				return LiteralKind::Boolean{lit.symbol == kw::True};
			case ast::data::Literal::Err:
				return LiteralKind::Error{lit.symbol};
			}
		}
	 private:
		static Literal::KindUnion filterFloatLiteral(Symbol sym, std::optional<Symbol> suffix)
		{
			double val;
			LiteralKind::Float::KindUnion kind;
			if (suffix)
			{
				switch (suffix->data())
				{
				case sym::F32:
					kind = FloatType::Suffixed{FloatType::Suffixed::F32};
				case sym::F64:
					kind = FloatType::Suffixed{FloatType::Suffixed::F64};
				default:
					throw LiteralError(LiteralError::InvalidSuffix);
				}
			}
			else
			{
				kind = FloatType::Unsuffixed{};
			}
			return LiteralKind::Float{ std::stod(sym.toString().data(), nullptr), kind };
		}
		static Literal::KindUnion integerLiteral(Symbol sym, std::optional<Symbol> suffix = std::nullopt)
		{
			LiteralKind::Int::KindUnion type;
			uint32_t base = 10;
			auto s = sym.toString();
			if(s.starts_with("0b"))
			{
				base = 2;
			}
			else if(s.starts_with("0o"))
			{
				base = 8;
			}
			else if(s.starts_with("0x"))
			{
				base = 16;
			}
			if (suffix)
			{
				switch (suffix->data())
				{
				case sym::ISize:
					type = IntType::Signed{ IntType::Signed::ISize };
					break;
				case sym::I8:
					type = IntType::Signed{ IntType::Signed::I8 };
					break;
				case sym::I16:
					type = IntType::Signed{ IntType::Signed::I16 };
					break;
				case sym::I32:
					type = IntType::Signed{ IntType::Signed::I32 };
					break;
				case sym::I64:
					type = IntType::Signed{ IntType::Signed::I64 };
					break;
				case sym::I128:
					throw LiteralError{ LiteralError::NotImplemented };
				case sym::USize:
					type = IntType::Unsigned{ IntType::Unsigned::USize };
					break;
				case sym::U8:
					type = IntType::Unsigned{ IntType::Unsigned::U8 };
					break;
				case sym::U16:
					type = IntType::Unsigned{ IntType::Unsigned::U16 };
					break;
				case sym::U32:
					type = IntType::Unsigned{ IntType::Unsigned::U32 };
					break;
				case sym::U64:
					type = IntType::Unsigned{ IntType::Unsigned::U64 };
					break;
				case sym::U128:
					throw LiteralError{ LiteralError::NotImplemented };
				default:
					if(s.starts_with('f'))
					{
						return filterFloatLiteral(sym,suffix);
					}
					throw LiteralError{ LiteralError::InvalidSuffix };
				}
			}
			else
			{
				type = IntType::Unsuffixed{};
			}
			return LiteralKind::Int{std::stoul(s.data(),nullptr,base),type};
		}
	};
}

#endif //CORROSION_SRC_AST_NODE_LITERAL_HPP_
