#ifndef WHITESPACES_HPP
#define WHITESPACES_HPP

#include <QChar>
#include <QFile>
#include <QMap>
#include <QSet>
#include <QTextStream>
#include <exception>
namespace rustc
{

/* Rust spec:
U+0009 (horizontal tab, '\t')
U+000A (line feed, '\n')
U+000B (vertical tab)
U+000C (form feed)
U+000D (carriage return, '\r')
U+0020 (space, ' ')
U+0085 (next line)
U+200E (left-to-right mark)
U+200F (right-to-left mark)
U+2028 (line separator)
U+2029 (paragraph separator)
*/
/**
 * @brief The WhiteSpacesSymbols is enumerator of Rust lang whitespaces.
 */
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

} // namespace rustc

#endif // WHITESPACES_HPP
