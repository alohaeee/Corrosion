#ifndef TOKENS_HPP
#define TOKENS_HPP

namespace rustc
{

/**
 * @brief The TokenID enum of rust token table.
 */
enum class TokenID : int
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

//// Rust spec keywords
///* strict

////KW_AS : as
////KW_BREAK : break
////KW_CONST : const
////KW_CONTINUE : continue
////KW_CRATE : crate
////KW_ELSE : else
////KW_ENUM : enum
////KW_EXTERN : extern
////KW_FALSE : false
////KW_FN : fn
////KW_FOR : for
////KW_IF : if
////KW_IMPL : impl
////KW_IN : in
////KW_LET : let
////KW_LOOP : loop
////KW_MATCH : match
////KW_MOD : mod
////KW_MOVE : move
////KW_MUT : mut
////KW_PUB : pub
////KW_REF : ref
////KW_RETURN : return
////KW_SELFVALUE : self
////KW_SELFTYPE : Self
////KW_STATIC : static
////KW_STRUCT : struct
////KW_SUPER : super
////KW_TRAIT : trait
////KW_TRUE : true
////KW_TYPE : type
////KW_UNSAFE : unsafe
////KW_USE : use
////KW_WHERE : where
////KW_WHILE : while
//*/
///* reserved
// KW_ABSTRACT : abstract
// KW_BECOME : become
// KW_BOX : box
// KW_DO : do
// KW_FINAL : final
// KW_MACRO : macro
// KW_OVERRIDE : override
// KW_PRIV : priv
// KW_TYPEOF : typeof
// KW_UNSIZED : unsized
// KW_VIRTUAL : virtual
// KW_YIELD : yield
//*/
///* weak
// KW_UNION : union
// KW_STATICLIFETIME : 'static

// Lexer 2015
// KW_DYN : dyn
//*/

} // namespace rustc
#endif // TOKENS_HPP
