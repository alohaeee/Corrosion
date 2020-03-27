#ifndef COMPILER_FIXTURE_HPP
#define COMPILER_FIXTURE_HPP

#include "compiler.hpp"
#include <gtest/gtest.h>

class CompilerFixture : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        compiler = new corrosion::Compiler();
        lexer = &compiler->GetLexer();
    }
    virtual void TearDown() override
    {
        delete compiler;
    }

    void TokenCheck(corrosion::TokenID expect, std::string_view str)
    {
        ASSERT_FALSE(lexer->isEmpty());
        auto token = lexer->GetToken();
        EXPECT_EQ(int(expect), int(token.type));
        EXPECT_STREQ(str.data(), token.lexem.toStdString().data());
    }
    void LexAndLog(std::string_view path)
    {
        auto text = corrosion::FileSystem::ReadAllFile(path.data());
        lexer->Lex(text);
        lexer->LogErr();
        lexer->Clear();
    }

    corrosion::Compiler *compiler;
    corrosion::Lexer<corrosion::TokenID> *lexer;
};

#endif // COMPILER_HPP
