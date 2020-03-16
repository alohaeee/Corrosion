#ifndef TOKENS_HPP
#define TOKENS_HPP

namespace corrosion
{

/**
 * @brief The TokenID enum of rust token table.
 * https://doc.rust-lang.org/reference/tokens.html
 */
enum class TokenID : unsigned int
{
    // use ASCII values for single char tokens
    SEMI = ';',
    COLON = ':',
    LPARENTHESIS = '(',
    RPARENTHESIS = ')',
    EQ = '=',
    STAR = '*',
    SLASH = '/',
    PLUS = '+',
    MINUS = '-',
    LBRAСKET = '[',
    RBRACKET = ']',
    LBRACER = '{',
    RBRACER = '}',
    CARET = '^',
    NOT = '!',
    PERCENT = '%',
    OR = '|',
    GT = '>',
    LT = '<',
    AT = '@',
    UNDERSCOR = '_',
    DOT = '.',
    COMMA = ',',
    POUND = '#',
    DOLLAR = '$',
    QUESTION = '?',

    // composite tokens
    PLUS_PLUS = 180, // ++
    MINUS_MINUS,     // --
    PLUS_EQ,         // +=
    MINUS_EQ,        // -=
    STAR_EQ,         // *=
    SLASH_EQ,        // /=
    PERCENT_EQ,      // %=
    CARET_EQ,        // ^=
    AND_EQ,          // &=
    OR_EQ,           // |=
    SHL_EQ,          // <<=
    SHR_EQ,          // >>=
    AND_AND,         // &&
    OR_OR,           // ||
    SHL,             // <<
    SHR,             // >>
    EQ_EQ,           // ==
    NE,              // !=
    GE,              // >=
    LE,              // <=
    DOT_DOT,         // ..
    DOT_DOT_DOT,     // ...
    DOT_DOT_EQ,      // ..=
    PATH_SEP,        // ::
    RARROW,          // ->
    FAT_ARROW,       // =>

    // KEY WORDS
    KW_WHILE = 230,
    KW_IF,
    KW_RETURN,

    // identifiers, constants, etc.
    IDENTIFIER = 280,
    // INTEGER_LITERAL,
    FLOAT_LITERAL,
    FLOAT_EXPONENT,
    STRING_LITERAL,
    BOOLEAN_LITERAL,
    DEC_LITERAL,
    OCT_LITERAL,
    BIN_LITERAL,
    HEX_LITERAL,
    TUPLE_INDEX,
    END = 349, // code used when at end of file
    UNKNOWN    // token was unrecognized by scanner

};

} // namespace corrosion
#endif // TOKENS_HPP
