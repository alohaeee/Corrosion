#include <chrono>
#include <iostream>
#include <fstream>
#include <gtest/gtest.h>

#include "corrosion.hpp"

using namespace corrosion;
struct Timer final
{
	Timer() : m_start{ std::chrono::system_clock::now() }
	{
	}

	void Elapsed()
	{
		auto now = std::chrono::system_clock::now();
		std::cerr << std::chrono::duration<double>(now - m_start).count() << " seconds" << std::endl;
	}

 private:
	std::chrono::time_point<std::chrono::system_clock> m_start;
};

std::string ReadWholeFile(std::string_view path)
{
	std::ifstream stream(path.data());

	std::string text;
	if (stream.is_open())
	{
		std::string str((std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>());

		stream.close();
		return str;
	}
	throw "no such file";
}

void Tokenize(std::string_view path)
{
	auto text = ReadWholeFile(path);

	Tokenizer tokenizer(text);

	std::size_t count = 0;
	std::cerr << ">>>>>>>>>>>>>>>>>>>!" << std::endl;
	std::cerr << "Start to tokenize file:" << path << std::endl;
	std::cerr << "Character count: " << text.size() << std::endl;
	Timer timer;
	while (!tokenizer.isEof())
	{
		tokenizer.advanceToken();
		count++;
	}
	std::cerr << "Time result: ";
	timer.Elapsed();
	std::cerr << "Tokens count: " << count << std::endl;
	std::cout << std::endl;
}

//void DebugPrint(std::string_view path)
//{
//	corrosion::Tokenizer tok{ ReadWholeFile(path) };
//
//	try
//	{
//		std::size_t last = 0;
//		std::size_t count = 0;
//		std::cerr << "Start to debug print file:" << path << std::endl;
//
//		while (!tok.isEof())
//		{
//			auto token = tok.advanceToken();
//			std::cerr << "Token Kind: " << std::quoted(Tokenizer::tokenKindPrintable(token.kind)) << " size: "
//					  << token.consumed << " text: "
//					  << std::quoted(tok.text().substr(last, token.consumed))
//					  << " Literal Kind: "
//					  << std::quoted(Tokenizer::literalKindPrintable(token.data))
//					  << " Where: " << last << std::endl;
//			last = tok.consumed();
//			count++;
//		}
//		std::cerr << "Tokens count: " << count << std::endl;
//	}
//	catch (EscapeError& error)
//	{
//		std::cerr << int(error) << std::endl;
//	}
//}

TEST(LEXER_BENCHMARK, lines400)
{
	Tokenize("rs/lexer/lex0.rs");
}
TEST(LEXER_BENCHMARK, lines600)
{
	Tokenize("rs/lexer/lex1.rs");
}
TEST(LEXER_BENCHMARK, lines1600)
{

	//! Have problems with raw strings. Infinite loop.

	//std::cerr << ReadWholeFile("rs/lexer/lex2.rs").substr(22355);
	//Tokenize("rs/lexer/lex2.rs");
	//DebugPrint("rs/lexer/lex2.rs");


}
