#ifndef CORROSION_SRC_PARSER_AST_TOKEN_HPP_
#define CORROSION_SRC_PARSER_AST_TOKEN_HPP_

#include "utility/std_incl.hpp"
#include <fmt/format.h>

#include "span/span.hpp"
#include "span/symbol.hpp"

namespace corrosion::ast
{

	enum class TokenKind : uint32_t
	{
		/* Expression-operator symbols. */
		Eq,
		Lt,
		Le,
		EqEq,
		Ne,
		Ge,
		Gt,
		AndAnd,
		OrOr,
		Not,
		Tilde,
		BinOp,
		BinOpEq,

		/* Structural symbols */
		At,
		Dot,
		DotDot,
		DotDotEq,
		//DotDotDot,
		Comma,
		Semi,
		Colon,
		ModSep,
		RArrow,
		FatArrow,
		Pound,
		Dollar,
		Question,
		/// An opening delimiter (e.g., `{`).
		OpenDelim,
		/// A closing delimiter (e.g., `}`).
		CloseDelim,

		/* Literals */
		Literal,

		/// Identifier token.
		/// Do not forget about `NtIdent` when you want to match on identifiers.
		/// It's recommended to use `Token::(ident,uninterpolate,uninterpolated_span)` to
		/// treat regular and interpolated identifiers in the same way.
		Ident,

		Type,
		/// Lifetime identifier token.
		/// Do not forget about `NtLifetime` when you want to match on lifetime identifiers.
		/// It's recommended to use `Token::(lifetime,uninterpolate,uninterpolated_span)` to
		/// treat regular and interpolated lifetime identifiers in the same way.
		Lifetime,

		//Interpolated,

//		// Can be expanded into several tokens.
//		/// A doc comment.
//		DocComment,

		// Junk. These carry no data because we don't really care about the data
		// they *would* carry, and don't really want to allocate a new ident for
		// them. Instead, users could extract that from the associated span.
		/// Whitespace.
		Whitespace,
		/// A comment.
		Comment,
		/// A completely invalid token which should be skipped.
		Unknown,

		Eof
	};
	namespace data
	{
		struct Delim
		{
			enum Kind
			{
				/// A round parenthesis (i.e., `(` or `)`).
				Paren,
				/// A square bracket (i.e., `[` or `]`).
				Bracket,
				/// A curly brace (i.e., `{` or `}`).
				Brace,
				/// An empty delimiter.
				NoDelim,
			} kind;

			std::size_t len() const noexcept
			{
				if (kind == Delim::Kind::NoDelim)
				{
					return 0;
				}
				return 1;
			}
			inline bool isEmpty() const noexcept
			{
				return kind == Delim::Kind::NoDelim;
			}
			std::string toString(bool open) const noexcept
			{
				switch (kind)
				{
				case Paren:
					return open ? "(" : ")";
				case Bracket:
					return open ? "[" : "]";
				case Brace:
					return open ? "{" : "}";
				default:
					return "";
				}
			}
			std::string toString() const noexcept
			{
				switch (kind)
				{
				case Paren:
					return "()";
				case Bracket:
					return "[]";
				case Brace:
					return "{}";
				default:
					return "";
				}
			}
		};
		struct BinOp
		{
			enum Kind
			{
				Plus,
				Minus,
				Star,
				Slash,
				Percent,
				Caret,
				And,
				Or,
				Shl,
				Shr
			} kind;
			std::string toString()
			{
				switch(kind)
				{
				case Plus:
					return "+";
				case Minus:
					return "-";
				case Star:
					return "*";
				case Slash:
					return "/";
				case Percent:
					return "%";
				case Caret:
					return "^";
				case And:
					return "&";
				case Or:
					return "|";
				case Shl:
					return "<<";
				case Shr:
					return ">>";
				default:
					return "";
				}
			}
		};

		struct Literal
		{
			enum Kind
			{
				Bool, // AST only, must never appear in a `Token`
				Byte,
				Char,
				Integer,
				Float,
				Str,
				Err,
			} kind;
			Symbol symbol;
			std::optional<Symbol> suffix;

			std::string kindPrintable() const noexcept
			{
				switch (kind)
				{
				case Kind::Bool:
					return "bool";
				case Kind::Byte:
					return "byte";
				case Kind::Char:
					return "char";
				case Kind::Str:
					return "string";
				case Kind::Float:
					return "float";
				case Kind::Integer:
					return "integer";
				case Kind::Err:
					return "err";
				default:
					return "";
				}
			}
			bool mayHaveSuffix() const noexcept
			{
				if (kind == Kind::Integer || kind == Kind::Float || kind == Kind::Err)
				{
					return true;
				}
				return false;
			}
		};
		struct Ident
		{
			Symbol symbol;
			static bool identCanBeginExpr(Span span, Symbol symbol);
			static bool identCanBeginType(Span span, Symbol symbol);
			static bool exprKeyword(Symbol symbol)
			{
				switch (symbol.data())
				{
				case kw::Async:
				case kw::Do:
				case kw::Box:
				case kw::Break:
				case kw::Continue:
				case kw::False:
				case kw::For:
				case kw::If:
				case kw::Let:
				case kw::Loop:
				case kw::Match:
				case kw::Move:
				case kw::Return:
				case kw::True:
				case kw::Unsafe:
				case kw::While:
				case kw::Yield:
				case kw::Static:
					return true;
				default:
					return false;
				}
			}
			static bool typeKeyword(Symbol symbol)
			{
				switch (symbol.data())
				{
				case kw::Underscore:
				case kw::For:
				case kw::Impl:
				case kw::Fn:
				case kw::Unsafe:
				case kw::Extern:
				case kw::Typeof:
				case kw::Dyn:
					return true;
				default:
					return false;
				}
			}
		};

		struct Lifetime
		{
			Symbol symbol;
		};
		struct Empty
		{

		};
	}
	using TokenData = std::variant<data::Delim,
								   data::BinOp,
								   data::Literal,
								   data::Ident,
								   data::Lifetime,
								   data::Empty>;

	struct Token
	{
		TokenKind kind;
		Span span;
		TokenData data;

	 public:
		Token(TokenKind kind = TokenKind::Whitespace, Span span = {}, TokenData data = data::Empty{}) :
			kind{ kind }, span{ span }, data{ data }
		{
		}
		bool isOp() const noexcept
		{
			switch (kind)
			{
			case TokenKind::OpenDelim:
			case TokenKind::CloseDelim:
			case TokenKind::Literal:
			case TokenKind::Comment:
			case TokenKind::Ident:
			case TokenKind::Lifetime:
			case TokenKind::Whitespace:
			case TokenKind::Eof:
				return false;
			default:
				return true;
			}
		}
		template<typename T>
		inline auto getData() const
		{
			return std::get<T>(data);
		}
		bool isLikePlus() const noexcept
		{
			if (kind == TokenKind::BinOp || kind == TokenKind::BinOpEq)
			{
				auto kind = std::get<data::BinOp>(data).kind;
				if (kind == data::BinOp::Kind::Plus)
				{
					return true;
				}
			}
			return false;
		}
		//! Returns `true` if the token can appear at the start of an expression.
		bool canBeginExpr() const
		{
			switch (kind)
			{
			case TokenKind::Ident: // value name or keyword
			{
				auto token_data = getData<data::Ident>();
				return data::Ident::identCanBeginExpr(span, token_data.symbol);
			}
			case TokenKind::OpenDelim:  // tuple, array or block
			case TokenKind::Literal: // literal
			case TokenKind::Not:  // operator not
				return true;
			case TokenKind::BinOp:
			{
				auto token_data = getData<data::BinOp>();
				switch (token_data.kind)
				{
				case data::BinOp::Minus: // unary minus
				case data::BinOp::Star: // dereference
				case data::BinOp::Or: // closure
				case data::BinOp::And: // reference
				case data::BinOp::Shl: // associated path
					return true;
				default:
					return false;
				}
			}
			case TokenKind::OrOr: // closure
			case TokenKind::AndAnd:  // double reference
			case TokenKind::DotDot:  // range notation
			case TokenKind::DotDotEq: // range notation
			case TokenKind::Lt: // associated path
			case TokenKind::ModSep: // global path
			case TokenKind::Lifetime: // labeled loop
			case TokenKind::Pound: // expression attributes
				return true;
			}
			return false;
		}
		//! Returns `true` if the token can appear at the start of a type.
		bool canBeginType() const
		{
			switch (kind)
			{
			case TokenKind::Ident: // type name or keyword
			{
				auto token_data = getData<data::Ident>();
				return data::Ident::identCanBeginType(span, token_data.symbol);
			}
			case TokenKind::OpenDelim:
			{
				auto token_data = getData<data::Delim>();
				switch (token_data.kind)
				{
				case data::Delim::Paren: // tuple
				case data::Delim::Bracket:  // array
					return true;
				default:
					return false;
				}
			}
			case TokenKind::BinOp:
			{
				auto token_data = getData<data::BinOp>();
				switch (token_data.kind)
				{
				case data::BinOp::Star: // raw pointer
				case data::BinOp::And: // reference
				case data::BinOp::Shl: // associated path
					return true;
				default:
					return false;
				}
			}
			case TokenKind::AndAnd: // double reference
			case TokenKind::Not: // never
			case TokenKind::Question: // question
			case TokenKind::Lifetime: // lifetime bound in trait object
			case TokenKind::Lt: // associated path
			case TokenKind::ModSep: // global path
				return true;
			default:
				return false;

			}
		}
		bool isLiteral() const noexcept
		{
			return kind == TokenKind::Literal;
		}
		bool isIdent() const noexcept
		{
			return kind == TokenKind::Ident;
		}
		bool isKeyword(std::uint32_t sym)
		{
			if (isIdent())
			{
				auto sym = getData<data::Ident>().symbol;
				if (sym == sym && sym.isKeyword())
				{
					return true;
				}
			}
			return false;
		}
		bool isKeyword()
		{
			if (isIdent())
			{
				auto sym = getData<data::Ident>().symbol;
				if (sym.isKeyword())
				{
					return true;
				}
			}
			return false;
		}
		bool isMutability() noexcept
		{
			if (isIdent())
			{
				auto symbol = std::get<data::Ident>(data).symbol;
				if (symbol == kw::Const || symbol == kw::Mut)
				{
					return true;
				}
			}
			return false;
		}

		inline bool isRangeKind() noexcept
		{
			switch (kind)
			{
			case TokenKind::DotDot:
			case TokenKind::DotDotEq:
				return true;
			}
			return false;
		}

		std::optional<Token> glue(Token joint)
		{
			TokenKind kind = TokenKind::Unknown;
			TokenData data;
			switch (this->kind)
			{
			case TokenKind::Eq:
			{
				switch (joint.kind)
				{
				case TokenKind::Eq:
					kind = TokenKind::EqEq;
					break;
				case TokenKind::Gt:
					kind = TokenKind::FatArrow;
					break;
				default:
					break;
				}
				break;
			}
			case TokenKind::Lt:
			{
				switch (joint.kind)
				{
				case TokenKind::Eq:
					kind = TokenKind::Le;
					break;
				case TokenKind::Lt:
					kind = TokenKind::BinOp;
					data = data::BinOp{ data::BinOp::Shl };
					break;
				case TokenKind::Le:
					kind = TokenKind::BinOpEq;
					data = data::BinOp{ data::BinOp::Shl };
					break;
				default:
					break;
				}
				break;
			}
			case TokenKind::Gt:
			{
				switch (joint.kind)
				{
				case TokenKind::Eq:
					kind = TokenKind::Ge;
					break;
				case TokenKind::Gt:
					kind = TokenKind::BinOp;
					data = data::BinOp{ data::BinOp::Shr };
					break;
				case TokenKind::Ge:
					kind = TokenKind::BinOpEq;
					data = data::BinOp{ data::BinOp::Shr };
					break;
				default:
					break;
				}
				break;
			}
			case TokenKind::Not:
			{
				switch (joint.kind)
				{
				case TokenKind::Eq:
					kind = TokenKind::Ne;
					break;
				default:
					break;
				}
				break;
			}
			case TokenKind::BinOp:
			{
				auto this_op = this->getData<data::BinOp>();
				switch (joint.kind)
				{
				case TokenKind::Eq:
					kind = TokenKind::Eq;
					data = this->data;
					break;
				case TokenKind::BinOp:
				{
					auto joint_op = joint.getData<data::BinOp>();
					if (joint_op.kind == this_op.kind)
					{
						if (joint_op.kind == data::BinOp::And)
						{
							kind = TokenKind::AndAnd;
						}
						else if (joint_op.kind == data::BinOp::Or)
						{
							kind = TokenKind::OrOr;
						}
					}
					break;
				}
				case TokenKind::Gt:
				{
					if (this_op.kind == data::BinOp::Minus)
					{
						kind = TokenKind::RArrow;
					}
					break;
				}
				default:
					break;
				}
				break;
			}
			case TokenKind::Colon:
			{
				if (joint.kind == TokenKind::Colon)
				{
					kind = TokenKind::ModSep;
				}
				break;
			}
			case TokenKind::Dot:
			{
				switch (joint.kind)
				{
				case TokenKind::Dot:
					kind = TokenKind::DotDot;
					break;
				default:
					break;
				}
				break;
			}
			case TokenKind::DotDot:
			{
				switch (joint.kind)
				{
				case TokenKind::Eq:
					kind = TokenKind::DotDotEq;
					break;
				default:
					break;
				}
				break;
			}
			default:
				break;


			}
			if(kind == TokenKind::Unknown)
			{
				return std::nullopt;
			}
			return Token(kind, Span::sum(this->span, joint.span), data);

		}
		std::string printable()
		{
			switch(kind)
			{
			case TokenKind::Eq:
				return "=";
			case TokenKind::Lt:
				return "<";
			case TokenKind::Le:
				return "<=";
			case TokenKind::EqEq:
				return "==";
			case TokenKind::Ne:
				return "!=";
			case TokenKind::Ge:
				return ">";
			case TokenKind::Gt:
				return ">=";
			case TokenKind::AndAnd:
				return "&&";
			case TokenKind::OrOr:
				return "||";
			case TokenKind::Not:
				return "!";
			case TokenKind::Tilde:
				return "~";
			case TokenKind::BinOp:
			{
				return this->getData<data::BinOp>().toString();
			}

			case TokenKind::BinOpEq:
			{
				return fmt::format("{}=",this->getData<data::BinOp>().toString());
			}

					/* Structural symbols */
			case TokenKind::At:
				return "at";
			case TokenKind::Dot:
				return ".";
			case TokenKind::DotDot:
				return "..";
			case TokenKind::DotDotEq:
				return "..=";
					//DotDotDot,
			case TokenKind::Comma:
				return ",";
			case TokenKind::Semi:
				return ":";
			case TokenKind::Colon:
				return ";";
			case TokenKind::ModSep:
				return "::";
			case TokenKind::RArrow:
				return "->";
			case TokenKind::FatArrow:
				return "=>";
			case TokenKind::Pound:
				return "#";
			case TokenKind::Dollar:
				return "$";
			case TokenKind::Question:
				return "?";
					/// An opening delimiter (e.g., `{`).
			case TokenKind::OpenDelim:
				return getData<data::Delim>().toString(true);
					/// A closing delimiter (e.g., `}`).
			case TokenKind::CloseDelim:
				return getData<data::Delim>().toString(false);

					/* Literals */
			case TokenKind::Literal:
			{
				auto lit = getData<data::Literal>();
				return fmt::format("Literal: {} Sym: {} {}", lit.kindPrintable(),lit.symbol.toString(),
					(lit.suffix?(std::string("Suffix: ") + std::string(lit.suffix->toString())):""));
			}


					/// Identifier token.
					/// Do not forget about `NtIdent` when you want to match on identifiers.
					/// It's recommended to use `Token::(ident,uninterpolate,uninterpolated_span)` to
					/// treat regular and interpolated identifiers in the same way.
			case TokenKind::Ident:
			{
				return "Ident Sym:"+std::string(getData<data::Ident>().symbol.toString());
			}

			case TokenKind::Type:
			{
				return "type";
			}
					/// Lifetime identifier token.
					/// Do not forget about `NtLifetime` when you want to match on lifetime identifiers.
					/// It's recommended to use `Token::(lifetime,uninterpolate,uninterpolated_span)` to
					/// treat regular and interpolated lifetime identifiers in the same way.
			case TokenKind::Lifetime:
			{
				return "Lifetime: " + std::string(getData<data::Lifetime>().symbol.toString());
			}

					//Interpolated,

//		// Can be expanded into several tokens.
//		/// A doc comment.
//		DocComment,

					// Junk. These carry no data because we don't really care about the data
					// they *would* carry, and don't really want to allocate a new ident for
					// them. Instead, users could extract that from the associated span.
					/// Whitespace.
			case TokenKind::Whitespace:

				return "Whitespace";

					/// A comment.
			case TokenKind::Comment:
				return "Comment";
					/// A completely invalid token which should be skipped.
			case TokenKind::Unknown:
				return "Unknown";

			case TokenKind::Eof:
				return "Eof";
			default:
				return "";
			}
		}

	};
}

#endif //CORROSION_SRC_PARSER_AST_TOKEN_HPP_
