#include <gtest/gtest.h>

#include "compiler.hpp"
#include "lexer/compiler_fixture.hpp"
#include "lexer/rxmap.hpp"
#include "lexer/tokenid.hpp"
#include <iostream>
#include <random>

using namespace corrosion;

TEST(RegExpTest, HexDigit)
{
    QRegExp rg(regexp::number_literals::HEX_DIGIT);
    for (int i = '0'; i <= '9'; i++)
    {
        EXPECT_TRUE(rg.exactMatch(QChar(i)));
    }
    for (int i = 'a', j = 'A'; i <= 'f'; i++, j++)
    {
        EXPECT_TRUE(rg.exactMatch(QChar(i)));
        EXPECT_TRUE(rg.exactMatch(QChar(j)));
    }
    for (int i = 'g', j = 'G'; i <= 'z'; i++, j++)
    {
        EXPECT_FALSE(rg.exactMatch(QChar(i)));
        EXPECT_FALSE(rg.exactMatch(QChar(j)));
    }

    EXPECT_FALSE(rg.exactMatch("Simple Text"));
}
TEST(RegExpTest, DecDigit)
{

    QRegExp rg(regexp::number_literals::DEC_DIGIT);
    for (int i = '0'; i <= '9'; i++)
    {
        EXPECT_TRUE(rg.exactMatch(QChar(i)));
    }
    for (int i = '9' + 1; i <= 255; i++)
    {
        EXPECT_FALSE(rg.exactMatch(QChar(i)));
    }

    EXPECT_FALSE(rg.exactMatch("Simple Text"));
}
TEST(RegExpTest, DecLiteral)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> uid;
    QRegExp rg(regexp::number_literals::DEC_LITERAL);
    for (int i = 0; i <= 20; i++)
    {
        EXPECT_TRUE(rg.exactMatch(QString::number(uid(gen))));
    }
    for (int i = '9' + 1; i <= 255; i++)
    {
        EXPECT_FALSE(rg.exactMatch(QChar(i)));
    }

    EXPECT_TRUE("123");
    EXPECT_TRUE("1_");
    EXPECT_FALSE(rg.exactMatch("Simple Text"));
}

TEST_F(CompilerFixture, NumberLiterals)
{
    lexer->Lex(
        "123; 123i32; 123_u16 let a: u64 = 123; 0b1111_1111_1001_0000;  0b1111_1111_1001_0000i128; 0o________1;");
    TokenCheck(TokenID::INTEGER_LITERAL, "123");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::INTEGER_LITERAL, "123i32");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::INTEGER_LITERAL, "123_u16");

    TokenCheck(TokenID::KW_LET, "let");
    TokenCheck(TokenID::IDENTIFIER, "a");
    TokenCheck(TokenID::COLON, ":");
    TokenCheck(TokenID::INTEGER_SUFFIX, "u64");
    TokenCheck(TokenID::EQ, "=");
    TokenCheck(TokenID::INTEGER_LITERAL, "123");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::INTEGER_LITERAL, "0b1111_1111_1001_0000");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::INTEGER_LITERAL, "0b1111_1111_1001_0000i128");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::INTEGER_LITERAL, "0o________1");
    TokenCheck(TokenID::SEMI, ";");

    lexer->Lex("0b0102; 123AFB43 0o0581; 0b_; 0x____");
    TokenCheck(TokenID::UNKNOWN, "0b0102");
    TokenCheck(TokenID::SEMI, ";");
    TokenCheck(TokenID::UNKNOWN, "123AFB43");
    TokenCheck(TokenID::UNKNOWN, "0o0581");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::UNKNOWN, "0b_");
    TokenCheck(TokenID::SEMI, ";");
    TokenCheck(TokenID::UNKNOWN, "0x____");

    LexAndLog("rs/lexer/integer_literals.rs");
}
TEST_F(CompilerFixture, FloatLiterals)
{
    QRegExp rx(regexp::number_literals::FLOAT_LITERAL);
    EXPECT_FALSE(rx.exactMatch("0o0102"));
    EXPECT_TRUE(rx.exactMatch("0.0102"));

    lexer->Lex("123.0f64; 0.1f64 let x: f32 = 2.; 123f32 0.123 .123");

    TokenCheck(TokenID::FLOAT_LITERAL, "123.0f64");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::FLOAT_LITERAL, "0.1f64");

    TokenCheck(TokenID::KW_LET, "let");
    TokenCheck(TokenID::IDENTIFIER, "x");
    TokenCheck(TokenID::COLON, ":");
    TokenCheck(TokenID::FLOAT_SUFFIX, "f32");
    TokenCheck(TokenID::EQ, "=");
    TokenCheck(TokenID::FLOAT_LITERAL, "2.");
    TokenCheck(TokenID::SEMI, ";");

    TokenCheck(TokenID::FLOAT_LITERAL, "123f32");
    TokenCheck(TokenID::FLOAT_LITERAL, "0.123");

    TokenCheck(TokenID::DOT, ".");
    TokenCheck(TokenID::INTEGER_LITERAL, "123");

    LexAndLog("rs/lexer/float_literals.rs");
}

// TEST(RegExpTest, FloatExponent)
//{
//    QRegExp rx(regexp::number_literals::FLOAT_EXPONENT);

//    EXPECT_TRUE(rx.exactMatch("E9"));
//    //    EXPECT_TRUE(rx.exactMatch("E+23"));
//    //    EXPECT_TRUE(rx.exactMatch("E-23"));

//    //    EXPECT_TRUE(rx.exactMatch("e-_23_"));
//    EXPECT_TRUE(rx.exactMatch("e______2"));

//    EXPECT_FALSE(rx.exactMatch("e_______"));
//    EXPECT_FALSE(rx.exactMatch("z_23_"));
//    EXPECT_FALSE(rx.exactMatch("E++_23_"));
//}
