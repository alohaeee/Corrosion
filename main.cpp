#include "lexer/fwd.hpp"
#include "lexer/lexer.hpp"
#include "lexer/rustlexerspec.hpp"

#include <iostream>

using namespace corrosion;
int main(int argc, char *argv[])
{
    Lexer<TokenID> lexer{};
    lexer.SetPackedTokens({TokenID::UNKNOWN,
                           {
                               {"while", TokenID::KW_WHILE},
                               {"if", TokenID::KW_IF},
                               {"return", TokenID::KW_RETURN},
                               {"(true)|(false)", TokenID::BOOLEAN_LITERAL},
                               {number_literals::DEC_LITERAL, TokenID::DEC_LITERAL},
                               {"[+-]?([0-9]*[.])?[0-9]+", TokenID::FLOAT_LITERAL},
                               {"\"([^\\\\]|\\\\.)*\"", TokenID::STRING_LITERAL},
                               {"[a-zA-Z_][a-zA-Z0-9_]*", TokenID::IDENTIFIER},

                           }});
    lexer.SetTerminateTokens({TokenID::UNKNOWN,
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
    lexer.Lex(" +=s // sus //s \n /*  ds */ */ u8 i = \"dsds\"; float j = .031;  @@#(I@)  !SD while(!) ! (sus) { int i "
              "= 3432; return; }s");

    lexer.Log();
    return 0;
}
