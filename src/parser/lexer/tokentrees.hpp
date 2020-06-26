#ifndef CORROSION_SRC_PARSER_LEXER_TOKENTREES_HPP_
#define CORROSION_SRC_PARSER_LEXER_TOKENTREES_HPP_

#include "string_reader.hpp"
#include "ast/token_tree.hpp"

namespace corrosion
{
	struct TokenTreeReader
	{
		TokenTreeReader(StringReader&& stringReader): reader{stringReader}
		{

		}
		TokenStream parseAllTokenTrees()
		{
			TokenStream buf;

			this->realToken();
			while (this->token.kind != ast::TokenKind::Eof)
			{
				if (auto token_tree = parseTokenTree(); token_tree)
				{
					if(!token_tree.has_value())
					{
						CR_DEBUG_LOG_TRACE("WTF");
					}
					buf.push(std::move(token_tree.value()));
				}
			}

			printAstTokenTree(buf);

			return buf;
		}
		TokenStream parseTokenTreesUntilCloseDelim()
		{
			TokenStream buf;
			while (true)
			{
				if (token.kind == ast::TokenKind::CloseDelim)
				{
					return buf;
				}
				if (auto token_tree = parseTokenTree(); token_tree)
				{
					buf.push(std::move(token_tree.value()));
				}
				else
				{
					return buf;
				}
			}
		}
		std::optional<TreeAndJoint> parseTokenTree()
		{

			switch (token.kind)
			{
			case ast::TokenKind::Eof:
			{
				auto &&[delim,last_open_span] = openBraces.back();
				reader.parseSession->errorSpan(last_open_span, "this file contains an unclosed delimiter");
				return std::nullopt;
			}
			case ast::TokenKind::OpenDelim:
			{
				auto pre_span = token.span;
				auto open_data = token.getData<ast::data::Delim>();

				// Parse the open delimiter
				openBraces.push_back({ open_data, pre_span });
				this->realToken();

				// Parse the token trees within the delimiters.
				// We stop at any delimiter so we can try to recover if the user
				// uses an incorrect delimiter.
				auto tts = parseTokenTreesUntilCloseDelim();
				auto delim_span = DelimSpan(pre_span, token.span);
				if (token.kind == ast::TokenKind::CloseDelim)
				{
					auto close_data = token.getData<ast::data::Delim>();
					if (open_data.kind == close_data.kind)
					{
						// Parse the closing delimiter.
						this->realToken();
					}
					else
					{
						// Incorrect delimiter.
						reader.parseSession->errorSpan(token.span,
							fmt::format("expected {}, but found {}", open_data.toString(), close_data.toString()));
					}
				}
				else
				{
					return std::nullopt;
				}
				return TreeAndJoint{TokenTree{ Delimited{ delim_span, open_data, std::move(tts) }}, IsJoint::NotJoint};
			}
				// An unexpected closing delimiter (i.e., there is no
				// matching opening delimiter).
			case ast::TokenKind::CloseDelim:
			{
				auto delim = reader.parseSession->file().fromSpan(token.span);
				reader.parseSession->errorSpan(token.span, "unexpected closing delimiter");
				this->realToken();
				return std::nullopt;
			}
			default:
			{
				auto tt = TokenTree(std::move(token));
				this->realToken();
				bool is_joint = this->jointToPrev == IsJoint::Joint && this->token.isOp();
				return TreeAndJoint{ tt, (is_joint ? IsJoint::Joint : IsJoint::NotJoint) };
			}

			}
		}
		void realToken()
		{
			jointToPrev = IsJoint::Joint;
			while (true)
			{
				auto token = reader.nextToken();
				switch (token.kind)
				{
				case ast::TokenKind::Whitespace:
				case ast::TokenKind::Comment:
				case ast::TokenKind::Unknown:
					jointToPrev = IsJoint::NotJoint;
					break;
				default:
					this->token = token;
					return;
				}
			}
		}
	 private:
		inline void printAstTokenTree(const TokenStream& buf)
		{
#ifdef CR_ENABLE_LOG_STRREADER
			CR_LOG_STRREADER("[STRING READER] Token Tree in file: {}",reader.parseSession->file().path().string());
			printAstTokenTreeReq(buf,0);
#endif
		}
		void printAstTokenTreeReq(const TokenStream& buf,std::size_t level)
		{
			level++;
#ifdef CR_ENABLE_LOG_STRREADER
			for (auto &&[tt, joint]: buf.treeAndJoint)
			{
				if (tt.isToken())
				{
					auto tok = tt.getToken();
					CR_LOG_STRREADER("{0:>{1}}Span: {3}-{4} TokenKind: {2} ", "", (level-1)*3,
						 tok.printable(), tok.span.lo(), tok.span.hi());
				}
				else
				{
					auto delim = tt.getDelimited();
					CR_LOG_STRREADER("{0:>{1}}Span: {3}-{4} DelimKind: {2} ","", (level-1)*3,
						delim.kind.toString(), delim.span.open.lo(), delim.span.close.lo());
					printAstTokenTreeReq(delim.stream,level);
				}

			}


#endif
		}
	 public:

		IsJoint jointToPrev{IsJoint::NotJoint};
		std::vector<std::pair<ast::data::Delim, Span>> openBraces{};
		ast::Token token{};
		StringReader reader;
	};
}

#endif //CORROSION_SRC_PARSER_LEXER_TOKENTREES_HPP_
