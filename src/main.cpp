//! Log some parts of compiler into "logs" folder.
#define CR_ENABLE_DEBUG_LOG_ALL
//! Endless loop
#define TEST_INF_COMMANDS

#include "corrosion.hpp"

using namespace corrosion;

int main(int argc, char** argv)
{
	auto driver = Driver();

#ifndef TEST_INF_COMMANDS
	driver.mainArg(argc,argv);
#else
	CR_LOG_INFO("endless loop is activated\n");
	CR_LOG_TRACE("for quit type 'q'");
	std::string line;
	while (std::getline(std::cin, line))
	{
		auto args = ArgReader::splitByWhiteSpaces(line);
		if(args.size() == 1)
		{
			if(args.back() == "q")
			{
				break;
			}
		}
		driver.argEmit(args);
		CR_LOG_TRACE("finished last command; waiting for another one");
		CR_LOG_TRACE("for quit type 'q'");
	}
#endif
	return driver.ret();
}
