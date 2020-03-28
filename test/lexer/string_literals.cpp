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

TEST_F(CompilerFixture, CharLiteral)
{
    lexer->Lex("\'d\'; \'\\x21\' \'\\\"\' \'\n\'");

    TokenCheck(TokenID::CHAR_LITERAL, "\'d\'");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::CHAR_LITERAL, "\'\\x21\'");
    TokenCheck(TokenID::CHAR_LITERAL, "\'\\\"\'");

    TokenCheck(TokenID::UNKNOWN, "\'");
    TokenCheck(TokenID::UNKNOWN, "\'");
}

TEST_F(CompilerFixture, StringLiteral)
{
    lexer->Lex(" \"simple text\";\n \"alohaaeee\\neee\\t\"; \t \"\n\t\r\"");

    TokenCheck(TokenID::STRING_LITERAL, "\"simple text\"");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::STRING_LITERAL, "\"alohaaeee\\neee\\t\"");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::UNKNOWN, "\"");
    TokenCheck(TokenID::UNKNOWN, "\"");

    LexAndLog("rs/lexer/string_literals.rs");
}
