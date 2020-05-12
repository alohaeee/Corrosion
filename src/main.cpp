//! Log some parts of compiler into "logs" folder.
#define CR_ENABLE_DEBUG_LOG_ALL

#include "corrosion.hpp"
using namespace corrosion;

int main()
{
	Log::init();

	auto parseSession = std::make_shared<ParseSession>();
	parseSession->appendFile(SourceFile("test_file.txt", "sd s // sds \n fufuf[u] 's'bs   "
														 "/* sdd*/  \n<'a>\n\n sel\n \"yastroka\"123u32 's' '0x123' \"SUS AI"
														 " O S S0b123\"0x32 0b10101     0o6427 \t s { s  ss11s1 123 123.3"));
	CR_LOG_TRACE(parseSession->file().source().size());

	StringReader reader{ parseSession };
	while (reader.nextToken().kind != ast::TokenKind::Eof);

	// warn logging.
	try
	{
		CR_DEBUG_LOG_WARN("\n{}", parseSession->emitSpan({ 31, 146 }));
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	// interner.
	Interner::fresh();
	// TODO: make test for interner.
	CR_DEBUG_LOG_TRACE("Interner check: {}", Interner::get(sym::Char));
	return 0;
}
