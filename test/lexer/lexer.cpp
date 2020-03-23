#include <gtest/gtest.h>

#include "compiler.hpp"
#include "lexer/rxmap.hpp"
#include "lexer/tokenid.hpp"
#include <iostream>

using namespace corrosion;

class CompilerFixture : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        compiler = new Compiler();
    }
    virtual void TearDown() override
    {
        delete compiler;
    }

    Compiler *compiler;
};

TEST_F(CompilerFixture, RxMap)
{
    auto &lexer = compiler->GetLexer();
    lexer.Lex("while true;");
    auto s = lexer.ToList();
    std::cerr << s.size() << std::endl;
    std::cerr << s.first().lexem.toStdString() << std::endl;
    //    EXPECT_EQ(TokenID::KW_WHILE, lexer.GetToken().type);

    //    EXPECT_EQ(lexer.isEmpty(), false);
    // EXPECT_EQ(TokenID::BOOLEAN_LITERAL, lexer.GetToken().type);
}
