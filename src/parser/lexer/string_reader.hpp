#ifndef CORROSION_SRC_PARSER_LEXER_STRING_READER_HPP_
#define CORROSION_SRC_PARSER_LEXER_STRING_READER_HPP_

#include "utility/std_incl.hpp"

#include "lexer/lexer.hpp"
#include "parser/parse_session.hpp"
#include "ast/token.hpp"
#include "span/source_file.hpp"
#include "span/symbol.hpp"
#include "span/span.hpp"
#include "error/log.hpp"

namespace corrosion
{

	class StringReader
	{
	 public:
		StringReader(std::shared_ptr<ParseSession>& parseSession, std::size_t start = 0) : parseSession{
			parseSession }, m_start(start)
		{
			m_tokenizer = lexer::Tokenizer{ parseSession->file().source() };
		}

		inline void tokenizerLog(const lexer::Token& token)
		{
#ifdef CR_ENABLE_LOG_TOKENIZER
			Log::getTokenizerLogger()->flush();
			CR_LOG_TOKENIZER("[{}] TokenKind: \"{}\" Span: {}-{} {} \n\t\t Source: {}",
				parseSession->file().path().string(), lexer::Tokenizer::tokenKindPrintable(token.kind),
				m_start, m_tokenizer.consumed(),
				lexer::Tokenizer::tokenDataPrintable(token.data), m_tokenizer.text().substr(m_start, token.consumed));
#endif
		}
		inline std::string_view strToken() const
		{
			return m_tokenizer.text().substr(m_start,m_token.consumed);
		}
		inline std::string_view strTo(std::size_t n)
		{
//			std::cout << n << ":" << m_start << std::endl;
//			CR_DEBUG_LOG_TRACE("token:{} data:{}", m_tokenizer.tokenKindPrintable(m_token.kind),m_tokenizer.tokenDataPrintable(m_token.data));
			assert(n > m_start);
			return m_tokenizer.text().substr(m_start, n - m_start);
		}
		inline std::string_view strFrom(std::size_t n)
		{
			assert(n < m_tokenizer.consumed());
			return m_tokenizer.text().substr(n, m_tokenizer.consumed() - n);
		}
		inline Span spanToken() const
		{
			return { m_start, m_tokenizer.consumed() };
		}
		ast::Token nextToken()
		{
			if (m_tokenizer.isEof())
			{
				return ast::Token{ ast::TokenKind::Eof, { 0, 0 }};
			}

			m_token = m_tokenizer.advanceToken();

			// Logging into "tokenizer.txt" file if CR_ENABLE_LOG_TOKENIZER was defined.
			tokenizerLog(m_token);

			auto ast_token = lexerTokenCook(m_token);

			auto span = Span(m_start, m_token.consumed);
			m_start = m_tokenizer.consumed();

			return ast_token;
		}
		/// Turns simple `lexer::TokenKind` enum into a rich
		/// `ast::TokenKind`. This turns strings into interned
		/// symbols and runs additional validation.
		ast::Token lexerTokenCook(const lexer::Token& token)
		{
			ast::TokenData data;
			ast::TokenKind kind;
			switch (token.kind)
			{
			case lexer::TokenKind::LineComment:
				kind = ast::TokenKind::Comment;
				break;
			case lexer::TokenKind::BlockComment:
			{
				auto data = std::get<lexer::data::BlockComment>(token.data);
				if (!data.terminated)
				{
					parseSession->criticalSpan(spanToken(), "unterminated block comment");
				}
				kind = ast::TokenKind::Comment;
				break;
			}
			case lexer::TokenKind::Whitespace:
				kind = ast::TokenKind::Whitespace;
				break;

			case lexer::TokenKind::Identifier:
			{
				auto sym = Symbol::intern(strToken());
				kind = ast::TokenKind::Ident;
				data = ast::data::Ident{ sym };
				break;
			}

			case lexer::TokenKind::RawIdentifier:
				parseSession->errorSpan(spanToken(), "raw identifiers are not implimented.");
				kind = ast::TokenKind::Unknown;
				break;

			case lexer::TokenKind::Literal:
			{
				auto lit_data = std::get<lexer::data::LiteralKind>(token.data);
				data = lexerLiteralCook(lit_data);
				kind = ast::TokenKind::Literal;
				break;
			}

			case lexer::TokenKind::Lifetime:
			{
				auto start_with_number = std::get<lexer::data::Lifetime>(token.data).startWithNumber;
				if (start_with_number)
				{
					parseSession->errorSpan(spanToken(), "lifetimes cannot start with a number");
				}
				auto lifetime_ident = strToken();
				lifetime_ident = lifetime_ident.substr(1, lifetime_ident.size());
				auto sym = Symbol::intern(lifetime_ident);

				kind = ast::TokenKind::Lifetime;
				data = ast::data::Lifetime{ sym };
				break;
			}
			case lexer::TokenKind::Semi:
				kind = ast::TokenKind::Semi;
				break;
			case lexer::TokenKind::Comma:
				kind = ast::TokenKind::Comma;
				break;
			case lexer::TokenKind::Dot:
				kind = ast::TokenKind::Dot;
				break;
			case lexer::TokenKind::OpenParen:
				data = ast::data::Delim{ ast::data::Delim::Paren };
				kind = ast::TokenKind::OpenDelim;
				break;
			case lexer::TokenKind::CloseParen:
				data = ast::data::Delim{ ast::data::Delim::Paren };
				kind = ast::TokenKind::CloseDelim;
				break;
			case lexer::TokenKind::OpenBrace:
				data = ast::data::Delim{ ast::data::Delim::Brace };
				kind = ast::TokenKind::OpenDelim;
				break;
			case lexer::TokenKind::CloseBrace:
				data = ast::data::Delim{ ast::data::Delim::Brace };
				kind = ast::TokenKind::CloseDelim;
				break;
			case lexer::TokenKind::OpenBracket:
				data = ast::data::Delim{ ast::data::Delim::Bracket };
				kind = ast::TokenKind::OpenDelim;
				break;
			case lexer::TokenKind::CloseBracket:
				data = ast::data::Delim{ ast::data::Delim::Bracket };
				kind = ast::TokenKind::CloseDelim;
				break;
			case lexer::TokenKind::At:
				kind = ast::TokenKind::At;
				break;
			case lexer::TokenKind::Pound:
				kind = ast::TokenKind::Pound;
				break;
			case lexer::TokenKind::Tilde:
				kind = ast::TokenKind::Tilde;
				break;
			case lexer::TokenKind::Question:
				kind = ast::TokenKind::Question;
				break;
			case lexer::TokenKind::Colon:
				kind = ast::TokenKind::Colon;
				break;
			case lexer::TokenKind::Dollar:
				kind = ast::TokenKind::Dollar;
				break;
			case lexer::TokenKind::Eq:
				kind = ast::TokenKind::Eq;
				break;
			case lexer::TokenKind::Not:
				kind = ast::TokenKind::Not;
				break;
			case lexer::TokenKind::Minus:
				data = ast::data::BinOp{ ast::data::BinOp::Minus };
				kind = ast::TokenKind::BinOp;
				break;
			case lexer::TokenKind::And:
				data = ast::data::BinOp{ ast::data::BinOp::And };
				kind = ast::TokenKind::BinOp;
				break;
			case lexer::TokenKind::Or:
				data = ast::data::BinOp{ ast::data::BinOp::Or };
				kind = ast::TokenKind::BinOp;
				break;
			case lexer::TokenKind::Plus:
				data = ast::data::BinOp{ ast::data::BinOp::Plus };
				kind = ast::TokenKind::BinOp;
				break;
			case lexer::TokenKind::Star:
				data = ast::data::BinOp{ ast::data::BinOp::Star };
				kind = ast::TokenKind::BinOp;
				break;
			case lexer::TokenKind::Slash:
				data = ast::data::BinOp{ ast::data::BinOp::Slash };
				kind = ast::TokenKind::BinOp;
				break;
			case lexer::TokenKind::Caret:
				data = ast::data::BinOp{ ast::data::BinOp::Caret };
				kind = ast::TokenKind::BinOp;
				break;
			case lexer::TokenKind::Percent:
				data = ast::data::BinOp{ ast::data::BinOp::Percent };
				kind = ast::TokenKind::BinOp;
				break;
			case lexer::TokenKind::Gt:
				kind = ast::TokenKind::Gt;
				break;
			case lexer::TokenKind::Lt:
				kind = ast::TokenKind::Lt;
				break;


			default:
			{
				parseSession->errorSpan(spanToken(), "unknown start of token");
				return ast::TokenKind::Unknown;
			}
			}
			return { kind, spanToken(), data };
		}
		ast::data::Literal lexerLiteralCook(const lexer::data::LiteralKind kind)
		{
			return std::visit([this](auto&& arg) -> ast::data::Literal
			{
			  using T = std::decay_t<decltype(arg)>;
			  std::optional<Symbol> suf = std::nullopt;
			  if constexpr(std::is_same_v<T, lexer::literal_types::Char>)
			  {
				  auto content = Symbol::intern(strTo(arg.suffixStart));
				  if (!arg.terminated)
				  {
					  parseSession->criticalSpan(spanToken(), "unterminated character literal");
				  }
				  if (arg.suffixStart != m_tokenizer.consumed())
				  {
					  parseSession->errorSpan(spanToken(), "suffixes on a string literal are invalid");
					  suf = Symbol::intern(strFrom(arg.suffixStart));
				  }
				  return { ast::data::Literal::Char, content, suf };

			  }
			  else if constexpr(std::is_same_v<T, lexer::literal_types::Str>)
			  {
				  auto content = Symbol::intern(strTo(arg.suffixStart));
				  if (!arg.terminated)
				  {
					  parseSession->criticalSpan(spanToken(), "unterminated double quote string");
				  }
				  if (arg.suffixStart != m_tokenizer.consumed())
				  {
					  parseSession->errorSpan(spanToken(), "suffixes on a string literal are invalid");
					  suf = Symbol::intern(strFrom(arg.suffixStart));
				  }
				  return { ast::data::Literal::Str, content, suf };
			  }
			  else if constexpr(std::is_same_v<T, lexer::literal_types::Int>)
			  {
				  auto content = Symbol::intern(strTo(arg.suffixStart));
				  if (arg.emptyInt)
				  {
					  parseSession->errorSpan(spanToken(), "no valid digits found for number");
				  }
				  if (arg.suffixStart != m_tokenizer.consumed())
				  {
					  suf = Symbol::intern(strFrom(arg.suffixStart));
				  }

				  integerValidation(arg.base, strToken());
				  return { ast::data::Literal::Integer, content, suf };
			  }
			  else if constexpr(std::is_same_v<T, lexer::literal_types::Float>)
			  {
				  auto content = Symbol::intern(strTo(arg.suffixStart));
				  if (arg.emptyExponent)
				  {
					  parseSession->errorSpan(spanToken(), "expected at least one digit in exponent");
				  }
				  if (arg.suffixStart != m_tokenizer.consumed())
				  {
					  suf = Symbol::intern(strFrom(arg.suffixStart));
				  }
				  switch (arg.base)
				  {
				  case lexer::Base::Hexadecimal:
					  parseSession->errorSpan(spanToken(), "hexadecimal float literal is not supported");
					  break;
				  case lexer::Base::Octal:
					  parseSession->errorSpan(spanToken(), "octal float literal is not supported");
					  break;

				  case lexer::Base::Binary:
					  parseSession->errorSpan(spanToken(), "binary float literal is not supported");
					  break;
				  default:
					  break;
				  }
				  return { ast::data::Literal::Float, content, suf };
			  }
			  else
			  {
			  	static_assert("Bad visit in coocking ast literal");
			  }
			}, kind);
		}
		void integerValidation(lexer::Base base, std::string_view number)
		{
			uint8_t base_num;
			if (base == lexer::Base::Binary)
			{
				base_num = 2;
			}
			else if (base == lexer::Base::Binary)
			{
				base_num = 8;
			}
			else
			{
				return;
			}
			number = number.substr(2, number.size());
			for (auto& c: number)
			{
				if (static_cast<uint8_t>(c - '0') >= base_num)
				{
					parseSession->errorSpan(spanToken(), fmt::format("invalid digit for a base {} literal", base));
					return;
				}
			}
		}
		std::shared_ptr<ParseSession> parseSession;
	 protected:
		lexer::Tokenizer m_tokenizer{};
		std::size_t m_start;
		lexer::Token m_token{};

	};
}
#endif //CORROSION_SRC_PARSER_LEXER_STRING_READER_HPP_
