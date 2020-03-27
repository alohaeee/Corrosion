#include <gtest/gtest.h>

#include "compiler_fixture.hpp"
#include <iostream>
#include <string>

using namespace corrosion;

TEST(RegExpTest, AsciiEscapes)
{
    QRegExp rx("\\\\n");
    EXPECT_TRUE(rx.exactMatch("\\n"));
    auto file = FileSystem::ReadAllFile("rs/lexer/string_literals.rs");

    EXPECT_NE(-1, rx.indexIn(file));

    rx.setPattern(regexp::string_literals::ASCII_ESCAPES);

    EXPECT_TRUE(rx.exactMatch("\\n"));
    EXPECT_TRUE(rx.exactMatch("\\r"));
    EXPECT_TRUE(rx.exactMatch("\\t"));
    EXPECT_TRUE(rx.exactMatch("\\0"));
    EXPECT_TRUE(rx.exactMatch("\\x21"));

    EXPECT_NE(-1, rx.indexIn(file));
}

TEST(RegExpTest, QuoteEscapes)
{
    QRegExp rx(regexp::string_literals::QUOTE_ESCAPES);

    EXPECT_TRUE(rx.exactMatch("\\\'"));
    EXPECT_TRUE(rx.exactMatch("\\\""));

    EXPECT_FALSE(rx.exactMatch("\""));

    auto file = FileSystem::ReadAllFile("rs/lexer/string_literals.rs");
    EXPECT_NE(-1, rx.indexIn(file));
    EXPECT_STREQ("\\\'", rx.cap().toStdString().data());
}
