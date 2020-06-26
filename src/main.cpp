//! Log some parts of compiler into "logs" folder.
#define CR_ENABLE_DEBUG_LOG_ALL

#include "corrosion.hpp"

#include "ast/fwd.hpp"
using namespace corrosion;

int main()
{
	Log::init();
	Interner::fresh();

	auto parseSession = std::make_shared<ParseSession>();
	parseSession->appendFile(SourceFile("test_file.txt", "sd s // sds \n fufuf[u] 's'bs   "
														 "/* sdd*/  \n<'a>\n\n sel\n \"yastroka\"123u32 's' '0x123' \"SUS AI"
														 " O S S0b123\"0x32 0b10101     0o6427 \t s { s  ss11s1 123 123.3 } }\n} \n { sd "));
	CR_LOG_TRACE(parseSession->file().source().size());


	StringReader reader{ parseSession };
	TokenTreeReader treeReader{std::move(reader)};
	treeReader.parseAllTokenTrees();


//	Parser parser;
//
//	parser.parseAssocExprWith(0,nullptr);


	return 0;
}
