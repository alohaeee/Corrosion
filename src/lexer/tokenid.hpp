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
    // punctuations tokens
    SEMI,         // ;
    COLON,        // :
    LPARENTHESIS, // (
    RPARENTHESIS, // )
    EQ,           // =
    STAR,         // *
    SLASH,        // /
    PLUS,         // +
    MINUS,        // -
    LBRACKET,     // [
    RBRACKET,     // ]
    LBRACER,      // {
    RBRACER,      // }
    CARET,        // ^
    NOT,          // !
    PERCENT,      // %
    AND,          // &
    OR,           // |
    ANDAND,       // &&
    OROR,         // ||
    SHL,          // <<
    SHR,          // >>
    PLUSEQ,       // +=
    MINUSEQ,      // -=
    STAREQ,       // *=
    SLASHEQ,      // /=
    PERCENTEQ,    // %=
    CARETEQ,      // ^=
    ANDEQ,        // &=
    OREQ,         // |=
    GT,           // >
    LT,           // <
    AT,           // @
    UNDERSCORE,   // _
    DOT,          // .
    DOTDOT,       // ..
    DOTDOTDOT,    // ...
    DOTDOTEQ,     // ..=
    PATHSEP,      // ::
    RARROW,       // ->
    FATARROW,     // =>
    COMMA,        // ,
    POUND,        // #
    DOLLAR,       // $
    QUESTION,     // ?
    SHLEQ,        // <<=
    SHREQ,        // >>=
    EQEQ,         // ==
    NE,           // !=
    GE,           // >=
    LE,           // <=

    // KEY WORDS
    // strict
    KW_WHILE = 230,
    KW_FOR,
    KW_IF,
    KW_ELSE,
    KW_RETURN,
    KW_LET,
    KW_BREAK,
    KW_AS,
    KW_CONST,
    KW_CONTINUE,
    KW_CRATE,
    KW_ENUM,
    KW_EXTERN,
    KW_FALSE,
    KW_FN,
    KW_IMPL,
    KW_LOOP,
    KW_MATCH,     // match
    KW_MOD,       // mod
    KW_MOVE,      // move
    KW_MUT,       // mut
    KW_PUB,       // pub
    KW_REF,       // ref
    KW_SELFVALUE, // self
    KW_SELFTYPE,  // Self
    KW_STATIC,
    KW_STRUCT,
    KW_SUPER,
    KW_TRAIT,
    KW_TRUE,
    KW_TYPE,
    KW_UNSAFE,
    KW_USE,
    KW_WHERE,
    KW_ASYNC,
    KW_AWAIT,
    KW_DYN,
    // reserved
    KW_ABSTRACT,
    KW_BECOME,
    KW_BOX,
    KW_DO,
    KW_FINAL,
    KW_MACRO,
    KW_OVERRIDE,
    KW_PRIV,
    KW_TYPEOF,
    KW_UNSIZED,
    KW_VIRTUAL,
    KW_YIELD,
    KW_TRY,
    // weak
    KW_UNION,
    KW_STATICLIFETIME, // 'static

    // identifiers
    IDENTIFIER = 280,
    // literals
    INTEGER_LITERAL,
    INTEGER_SUFFIX,
    FLOAT_LITERAL,
    FLOAT_SUFFIX,
    FLOAT_EXPONENT,
    CHAR_LITERAL,
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
