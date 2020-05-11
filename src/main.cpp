//! Log some parts of compiler into "logs" folder.
#define CR_ENABLE_DEBUG_LOG_ALL

#include "corrosion.hpp"
#include <iomanip>
#include <Windows.h>
#include "utility/uchar.hpp"


using namespace corrosion;

int main()
{
	std::fstream fout("u8.txt", std::ios::trunc);

	std::string str((std::istreambuf_iterator<char>(fout)),
		std::istreambuf_iterator<char>());
	str.append("hehehe喜 ☼>↨5 ,бlЩе3И8сg╝,8*");

	fout << std::endl;
	fout << str;

	std::string test = "\xD8\x00\xDF\x48ሒввру喜";
	fout << test;

	SetConsoleOutputCP(CP_UTF8);

	fout.close();
	try{
		std::string s("ы喜sdа3");
		U8Core::is_valid(s);


		UChar cc;
		cc.replace<0>('d');
		std::cout << "REPLACED: " << cc <<  std::endl;



		auto crs = Chars(s);

		auto iter = ++crs.begin();

		auto end = crs.end();
		std::cout << "FIRST: " << *iter << " LEN: " << iter->len_utf8() << std::endl;
		iter++;
		std::cout << "FIRST: " << *iter << " LEN: " << iter->len_utf8() << std::endl;
		iter++;
		std::cout << "FIRST: " << *iter << " LEN: " << iter->len_utf8() << std::endl;
		iter++;
		std::cout << "FIRST: " << *iter << " LEN: " << iter->len_utf8() << std::endl;
		iter++;
		std::cout << "FIRST: " << *iter << " LEN: " << iter->len_utf8() << std::endl;
		std::cout << "RAVNI????" << (iter == end) << std::endl;
		iter++;
		std::cout << "FIRST: " << *iter << " LEN: " << iter->len_utf8() << std::endl;
		std::cout << "RAVNI????" << (iter == end) << std::endl;
		std::string newsss;

		auto testtt = "\xE2\x98\xAD";
		std::cout << "SSDS: " << testtt << std::endl;

		auto b = UChar::from_str_to_vec(s);
		for(auto& c: b)
		{
			std::cout << c << std::endl;
		}

		for(auto& s: crs)
		{
			auto f = s.data();
			f.bytes[s.len_utf8()-1] =(s.data().bytes[s.len_utf8()-1]+1);
			newsss.append(f.bytes);
			std::cout << "FIRST: " << s << std::endl;
		}
		std::cout << "NOVAYA STROKA: "<< newsss << std::endl;

		UChar c = UChar::from_str(s);
		c.data().bytes[1] = c.data().bytes[1] + 1;
		std::cout << "C: " << c << std::endl;
		std::cout << "STR: " << s << std::endl;
		std::cout << "LEN: " << U8Core::str_len(s) << std::endl;

		std::cout << "VALID : " << "YEA" << std::endl;
	}
	catch(U8Error& e)
	{
		std::cout << "VALID:" <<  e.valid_up_to() << '\t' << "LEN: " << e.error_len()  << std::endl;
	}
	catch(std::exception &e)
	{
		std::cout << e.what();
	}
	Log::init();

	auto parseSession = std::make_shared<ParseSession>();
	parseSession->appendFile(SourceFile("test_file.txt", "sd s // sds \n fufuf[u] 's'bs   "
												  "/* sdd*/  \n<'a>\n\n sel\n \"yastroka\"123u32 's' '0x123' \"SUS AI"
			  " O S S0b123\"0x32 0b10101     0o6427 \t s { s  ss11s1 123 123.3"));
	CR_LOG_TRACE(parseSession->file().source().size());

	StringReader reader{parseSession};
	while(reader.nextToken().kind != ast::TokenKind::Eof);
	try
	{
		CR_DEBUG_LOG_WARN("\n{}",parseSession->emitSpan({ 31, 146 }));
	}
	catch(std::exception& e)
	{
		std::cout << e.what();
	}

	Interner::fresh();
	CR_DEBUG_LOG_TRACE("AHAHA: {}",Interner::get(sym::Char));




	return 0;
}
