#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "lexer/lexer.hpp"
#include "lexer/numberliterals.hpp"
#include "parser/parser.hpp"

namespace rustc
{
class Compiler
{
    using constans = const QString;
    void InitLexer()
    {
        m_lexer.SetPackedTokens({TokenID::UNKNOWN,
                                 {
                                     {"while", TokenID::KW_WHILE},
                                     {"if", TokenID::KW_IF},
                                     {"return", TokenID::KW_RETURN},
                                     //{"[0-9]+", TokenID::INTEGER_LITERAL},
                                     {"[+-]?([0-9]*[.])?[0-9]+", TokenID::FLOAT_LITERAL},
                                     {"\"([^\\\\]|\\\\.)*\"", TokenID::STRING_LITERAL},
                                     {"[a-zA-Z_][a-zA-Z0-9_]*", TokenID::IDENTIFIER},
                                 }});
        m_lexer.SetTerminateTokens({TokenID::UNKNOWN,
                                    {{"\\(", TokenID::LPARENTHESIS},
                                     {"\\)", TokenID::RPARENTHESIS},
                                     {"\\{", TokenID::LBRACER},
                                     {"\\}", TokenID::RBRACER},
                                     {"=", TokenID::EQ},
                                     {";", TokenID::SEMI},
                                     {"\\+", TokenID::PLUS},
                                     {"-", TokenID::MINUS},
                                     {"\\*", TokenID::STAR},
                                     {"/", TokenID::SLASH},
                                     {"\\+=", TokenID::PLUS_EQ},
                                     {"-=", TokenID::MINUS_EQ},
                                     {"\\*=", TokenID::STAR_EQ},
                                     {"/=", TokenID::SLASH_EQ},
                                     {"\\++", TokenID::PLUS_PLUS},
                                     {"--", TokenID::MINUS_MINUS},
                                     // ignore multi line comments
                                     {"(/\\*.*\\*/)", Lexer<TokenID>::IGNORE},
                                     // ignore line comment
                                     {"//([^\n]*)(\n|$)", Lexer<TokenID>::IGNORE},
                                     // ignore white space
                                     {"[ \\n\\t\\r]", Lexer<TokenID>::IGNORE}}});
    }

public:
private:
    Lexer<TokenID> m_lexer;
};
} // namespace rustc
#endif // COMPILER_HPP
