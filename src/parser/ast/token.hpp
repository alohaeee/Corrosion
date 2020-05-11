#ifndef CORROSION_SRC_PARSER_AST_TOKEN_HPP_
#define CORROSION_SRC_PARSER_AST_TOKEN_HPP_

#include "utility/fwd.hpp"

#include "span/span.hpp"

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
		DotDotDot,
		DotDotEq,
		Comma,
		Semi,
		Colon,
		ModSep,
		RArrow,
		LArrow,
		FatArrow,
		Pound,
		Dollar,
		Question,
		/// Used by proc macros for representing lifetimes, not generated by lexer right now.
		SingleQuote,
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

		Interpolated,

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
		Shebang,
		/// A completely invalid token which should be skipped.
		Unknown,

		Eof
	};
	namespace data
	{
		struct DelimToken
		{
			enum class TokenKind
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
				if (kind == DelimToken::TokenKind::NoDelim)
				{
					return 0;
				}
				return 1;
			}
			inline bool isEmpty() const noexcept
			{
				return kind == DelimToken::TokenKind::NoDelim;
			}
		};

		struct LitKind
		{
			enum class TokenKind
			{
				Bool, // AST only, must never appear in a `Token`
				Byte,
				Char,
				Integer,
				Float,
				Str,
				Err,
			} kind;
		};
		struct Keyword
		{

		};
	}

	struct Token
	{
		TokenKind kind;
		Span span;
	};
}

#endif //CORROSION_SRC_PARSER_AST_TOKEN_HPP_
