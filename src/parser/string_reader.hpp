#ifndef CORROSION_SRC_PARSER_LEXER_STRING_READER_HPP_
#define CORROSION_SRC_PARSER_LEXER_STRING_READER_HPP_

#include "utility/fwd.hpp"

#include "lexer/lexer.hpp"
#include "parser/parse_session.hpp"
#include "parser/ast/token.hpp"
#include "span/source_file.hpp"
#include "span/symbol.hpp"
#include "span/span.hpp"
#include "error/log.hpp"

namespace corrosion
{

	class StringReader
	{
	 public:
		StringReader(std::shared_ptr<ParseSession>& parseSession, std::size_t start = 0) : m_parseSession{
			parseSession }, m_start(start)
		{
			m_tokenizer = Tokenizer{ m_parseSession->file().source() };
		}

		inline void tokenizerLog(const Token& token)
		{
#ifdef CR_ENABLE_LOG_TOKENIZER
			CR_LOG_TOKENIZER("[{}] TokenKind: \"{}\" Span: {}-{} {} \n\t\t Source: {}",
				m_parseSession->file().path().string(), Tokenizer::tokenKindPrintable(token.kind),
				m_start, m_tokenizer.consumed(),
				Tokenizer::tokenDataPrintable(token.data), m_tokenizer.text().substr(m_start, token.consumed));
#endif
		}
		inline std::string_view strToken() const
		{
			return m_tokenizer.text().substr(m_start, m_tokenizer.consumed());
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

			Token token = m_tokenizer.advanceToken();

			// Logging into "tokenizer.txt" file if CR_ENABLE_LOG_TOKENIZER was defined.
			tokenizerLog(token);

			auto span = Span(m_start, token.consumed);
			m_start = m_tokenizer.consumed();

			return ast::Token{ ast::TokenKind::BinOp, {}};
		}
		/// Turns simple `lexer::TokenKind` enum into a rich
		/// `ast::TokenKind`. This turns strings into interned
		/// symbols and runs additional validation.
		ast::TokenKind astTokenConvertion(const Token& token)
		{
			switch (token.kind)
			{
			case TokenKind::LineComment:
				return ast::TokenKind::Comment;
			case TokenKind::BlockComment:
			{
				auto data = std::get<token_data::BlockComment>(token.data);
				if (!data.terminated)
				{
					m_parseSession->criticalSpan(spanToken(), "unterminated block comment");
				}
				return ast::TokenKind::Comment;
			}
			case TokenKind::Whitespace:
				return ast::TokenKind::Whitespace;

			case TokenKind::Identifier:
			{
				auto sym = Symbol::intern(strToken());
				return ast::TokenKind::Ident;
			}

			case TokenKind::RawIdentifier:
				m_parseSession->errorSpan(spanToken(), "raw identifiers are not implimented.");
				return ast::TokenKind::Unknown;

			case TokenKind::Literal:
				return ast::TokenKind::Literal;

			case TokenKind::Lifetime:
			{
				auto start_with_number = std::get<token_data::Lifetime>(token.data).startWithNumber;
				if (start_with_number)
				{
					m_parseSession->errorSpan(spanToken(), "lifetimes cannot start with a number");
				}
				return ast::TokenKind::Lifetime;

			}
			default:
			{
				m_parseSession->criticalSpan(spanToken(), "unknown start of token");
				return ast::TokenKind::Unknown;
			}
			}
		}
		void checkLexerLiterals()
		{

		}
	 protected:
		Tokenizer m_tokenizer{};
		std::size_t m_start;
		std::shared_ptr<ParseSession> m_parseSession;
	};
}
#endif //CORROSION_SRC_PARSER_LEXER_STRING_READER_HPP_
