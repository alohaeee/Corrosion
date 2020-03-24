#include <gtest/gtest.h>

#include "compiler.hpp"
#include "lexer/rxmap.hpp"
#include "lexer/tokenid.hpp"
#include <iostream>
#include <random>

using namespace corrosion;

class CompilerFixture : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        compiler = new Compiler();
        lexer = &compiler->GetLexer();
    }
    virtual void TearDown() override
    {
        delete compiler;
    }

    Compiler *compiler;
    Lexer<TokenID> *lexer;
};
class RandFixture : public testing::Test
{
protected:
    virtual void SetUp() override
    {
    }
    virtual void TearDown() override
    {
        delete gen;
        delete rd;
    }
    std::random_device *rd;
    std::mt19937 *gen;
};

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
    auto text = FileSystem::ReadAllFile("rs/number_literals.rs");
    lexer->Lex(text);
    lexer->LogErr();
}
