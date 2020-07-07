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
	parseSession->appendFile(SourceFile("test_file.txt", "let a = (3+11u16+(23+4)*1)*2; \n { 1+1;}"));

	try
	{
		StringReader reader{ parseSession };
		TokenTreeReader treeReader{ std::move(reader) };
		auto stream = treeReader.parseAllTokenTrees();

		Parser parser{ parseSession, std::move(stream) };

		while(true)
		{
			if(auto stmt = parser.parseFullStmt();stmt)
			{
				stmt->printer(0);
			}
			else
			{
				break;
			}


		}
	}
	catch(CriticalException& e)
	{
		CR_LOG_TRACE(e.what());
	}

//
//	parser.parseAssocExprWith(0,nullptr);


	return 0;
}
