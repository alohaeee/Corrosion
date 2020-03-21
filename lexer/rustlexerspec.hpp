#ifndef RUSTLEXERSPEC_HPP
#define RUSTLEXERSPEC_HPP
#include <QString>

// Constans for lexer.
namespace corrosion
{

/**
 * @brief identifiers from rust spec:
 * @link https://doc.rust-lang.org/reference/identifiers.html @endlink
 * @note RAW_IDENTIFIERs are not implemented.
 */
namespace identifier
{
    constexpr const auto IDENTIFIER = "([a-zA-Z_][a-zA-Z0-9_]*)|(_[a-zA-Z0-9_]+)";
}
/**
 * @brief Number literals from rust spec:
 * @link https://doc.rust-lang.org/reference/tokens.html#number-literals @endlink
 */
namespace number_literals
{
    /**
     * @brief Integer literals from rust spec:
     * @link https://doc.rust-lang.org/reference/tokens.html#integer-literals @endlink
     */

    constexpr const auto HEX_DIGIT = "[0-9][a-f][A-F]";
    constexpr const auto NON_ZERO_DEC_DIGIT = "[1-9]";
    constexpr const auto DEC_DIGIT = "[0-9]";
    constexpr const auto OCT_DIGIT = "[0-7]";
    constexpr const auto BIN_DIGIT = "[0-1]";
    const QString HEX_LITERAL = QString("0x(%1|_)*%1(%1|_)*").arg(HEX_DIGIT);
    const QString OCT_LITERAL = QString("0o(%1|_)*%1(%1|_)*").arg(OCT_DIGIT);
    const QString BIN_LITERAL = QString("0o(%1|_)*%1(%1|_)*").arg(BIN_DIGIT);
    const QString TUPLE_INDEX = QString("0|%1%2*").arg(NON_ZERO_DEC_DIGIT).arg(DEC_DIGIT);
    const QString DEC_LITERAL = QString("%1(%1|_)*").arg(DEC_DIGIT);
    constexpr const auto INTEGER_SUFFIX = "(u8)|(u16)|(u32)|(u64)|(u128)|(usize)|(i8)|(i16)|(i32)|(i64)|(i128)|(isize)";
    const QString INTEGER_LITERAL = QString("((%1)|(%2)|(%3)|(%4)) %5")
                                        .arg(DEC_LITERAL)
                                        .arg(BIN_LITERAL)
                                        .arg(OCT_LITERAL)
                                        .arg(HEX_LITERAL)
                                        .arg(INTEGER_SUFFIX);

    /**
     * @brief Floating point literals from rust spec:
     * @link https://doc.rust-lang.org/reference/tokens.html#floating-point-literals @endlink
     */

    constexpr const auto FLOAT_SUFFIX = "(f32)|(f64)";
    const QString FLOAT_EXPONENT = QString("(e|E)(+|-)?(%1|_)*%1(%1|_)*").arg(DEC_LITERAL);
    constexpr const auto FLOAT_LITERAL = "[+-]?([0-9]*[.])?[0-9]+";

    /**
     * @brief Boolean literals from rust spec:
     * @link https://doc.rust-lang.org/reference/tokens.html#boolean-literals @endlink
     */

    constexpr const auto BOOLEAN_LITERAL = "(true)|(false)";
} // namespace number_literals

/**
 * @brief Comments but not like in rust spec, because they look like really weirdo shit
 * WHAT DOES THIS MEAN "~" ????.
 * @link https://doc.rust-lang.org/reference/comments.html @endlink
 * @note Doc comments are not implemented.
 */
namespace comments
{
    constexpr const auto LINE_COMMENT = "//([^\n]*)(\n|$)";
    constexpr const auto BLOCK_COMMENT = "(/\\*.*\\*/)";
} // namespace comments

/**
 * @brief White spaces from rust spec.
 * @link https://doc.rust-lang.org/reference/whitespace.html @endlink
 */
namespace whitespaces
{

    enum WhiteSpaceSymbols : int
    {
        HORIZONTAL_TAB = 0x09,
        LINE_FEED = 0x0A,
        VERTICAL_TAB = 0x0B,
        FORM_FEED = 0x0C,
        CARRIAGE_RETURN = 0x0D,
        SPACE = 0x20,
        NEXT_LINE = 0x85,
        LEFT_TO_RIGHT_MARK = 0xE2808E,
        RIGHT_TO_LEFT_MARK = 0xE2808F,
        LINE_SEPARATOR = 0xE280A8,
        PARAGRAPH_SEPARATOR = 0xE280A9
    };

    const QString WHITESPACE_REG = QString("[%1%2%3%4%5%6%7%8%9%10%11]")
                                       .arg(HORIZONTAL_TAB)
                                       .arg(LINE_FEED)
                                       .arg(VERTICAL_TAB)
                                       .arg(FORM_FEED)
                                       .arg(CARRIAGE_RETURN)
                                       .arg(SPACE)
                                       .arg(NEXT_LINE)
                                       .arg(LEFT_TO_RIGHT_MARK)
                                       .arg(RIGHT_TO_LEFT_MARK)
                                       .arg(LINE_SEPARATOR)
                                       .arg(PARAGRAPH_SEPARATOR);
} // namespace whitespaces

/**
 * @brief Character and string literals from rust spec.
 * @link https://doc.rust-lang.org/reference/tokens.html#character-and-string-literals @endlink
 * @note Raw string literals are not implemented.
 */
namespace string_literals
{
    constexpr const auto ISOLATED_CR = "\r^\n";
    const QString ASCII_ESCAPES =
        QString("(\\x%1%2)|[\n\r\t\\\0]").arg(number_literals::OCT_DIGIT).arg(number_literals::HEX_DIGIT);
    constexpr const auto QUOTE_ESCAPES = "[\'\"]";
    const QString UNICODE_ESCAPES = QString("\\u\\{(%1_*){1,6}\\}").arg(number_literals::HEX_DIGIT);

    const QString CHAR_LITERAL =
        QString("'(^['\\\n\r\t] | %1 | %2 | %3)'").arg(QUOTE_ESCAPES).arg(ASCII_ESCAPES).arg(UNICODE_ESCAPES);
    const constexpr auto STRING_CONTINUE = "\\\n";
    const QString STRING_LITERAL = QString("\"(^[\"\\%1]|(%2)|(%3)|(%4)|(%5))*\"")
                                       .arg(ISOLATED_CR)
                                       .arg(QUOTE_ESCAPES)
                                       .arg(ASCII_ESCAPES)
                                       .arg(UNICODE_ESCAPES)
                                       .arg(STRING_CONTINUE);

} // namespace string_literals
} // namespace corrosion

#endif // RUSTLEXERSPEC_HPP
