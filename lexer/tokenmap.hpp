#ifndef TOKENMAP_HPP
#define TOKENMAP_HPP

#include "fwd.hpp"
#include "tokenid.hpp"

namespace rustc
{
// inline TokenMap terminate{TokenID::UNKNOWN,
//                          {
//                              {"\\(", TokenID::LPARENTHESIS},
//                              {"\\)", TokenID::RPARENTHESIS},
//                              {"\\{", TokenID::LBRACER},
//                              {"\\}", TokenID::RBRACER},
//                              {"=", TokenID::ASSIGN},
//                              {";", TokenID::SEMICOLON},
//                              {"\\+", TokenID::PLUS},
//                              {"-", TokenID::MINUS},
//                              {"*", TokenID::STAR},
//                              {"/", TokenID::DIVIDE},
//                          }};
// inline TokenMap tokenMap{TokenID::UNKNOWN,
//                         {
//                             {"while", TokenID::KW_WHILE},
//                             {"if", TokenID::KW_IF},
//                             {"return", TokenID::KW_RETURN},
//                             {"\\+=", TokenID::PLUS_ASSIGN},
//                             {"-=", TokenID::MINUS_ASSIGN},
//                             {"*=", TokenID::MULTIPLY_ASSIGN},
//                             {"/=", TokenID::DIVIDE_ASSIGN},
//                             {"\\++", TokenID::PLUS_PLUS},
//                             {"--", TokenID::MINUS_MINUS},
//                             {"[0-9]+", TokenID::INTEGER},
//                             {"[a-zA-Z_][a-zA-Z0-9_]*", TokenID::IDENTIFIER},
//                         }};
} // namespace rustc

#endif // TOKENMAP_HPP
