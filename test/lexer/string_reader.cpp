#include <gtest/gtest.h>

#include "corrosion.hpp"

using namespace corrosion;
TEST(TOKENIZER, PUNCTUATION)
{
	Tokenizer tokenizer{ ";><!=*+-:.,@^[]{}/()%$~#?&|" };

	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Semi);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Gt);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Lt);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Not);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Eq);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Star);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Plus);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Minus);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Colon);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Dot);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Comma);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::At);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Caret);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::OpenBracket);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::CloseBracket);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::OpenBrace);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::CloseBrace);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Slash);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::OpenParen);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::CloseParen);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Percent);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Dollar);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Tilde);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Pound);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Question);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::And);
	ASSERT_EQ(tokenizer.advanceToken().kind, TokenKind::Or);

	ASSERT_TRUE(tokenizer.isEof());
}

TEST(TOKENIZER, FLOAT_LITERAL)
{
	std::string text = "123.0f64; 0.1f64 let x: f32 = 2.; 123f32 0.123 .123";
	Tokenizer tokenizer(text);
}
