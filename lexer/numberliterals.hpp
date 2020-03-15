#ifndef NUMBERLITERALS_HPP
#define NUMBERLITERALS_HPP
#include <QString>
namespace rustc
{
/**
 * @brief Integer literals from rust spec:
 * https://doc.rust-lang.org/reference/tokens.html#integer-literals
 */
namespace integer
{
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

} // namespace integer
/**
 * @brief Floating point literals from rust spec:
 * https://doc.rust-lang.org/reference/tokens.html#floating-point-literals
 */
namespace floating_point
{
constexpr const auto FLOAT_SUFFIX = "(f32)|(f64)";
const QString FLOAT_EXPONENT = QString("(e|E)(+|-)?(%1|_)*%1(%1|_)*").arg(integer::DEC_LITERAL);
constexpr const auto FLOAT_LITERAL = "[+-]?([0-9]*[.])?[0-9]+";

} // namespace floating_point

/**
 * @brief Boolean literals from rust spec:
 * https://doc.rust-lang.org/reference/tokens.html#boolean-literals
 */
namespace boolean
{
constexpr const auto BOOLEAN_LITERAL = "(true)|(false)";
}
} // namespace rustc

#endif // NUMBERLITERALS_HPP
