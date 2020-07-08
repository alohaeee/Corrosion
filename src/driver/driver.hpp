#ifndef CORROSION_SRC_DRIVER_DRIVER_HPP_
#define CORROSION_SRC_DRIVER_DRIVER_HPP_

#include "utility/std_incl.hpp"
#include "lexer/lexer.hpp"
#include "parser/interface.hpp"

namespace corrosion
{
	using CommandArgs = std::vector<std::string>;
	class ArgReader
	{
	 public:
		static CommandArgs splitByWhiteSpaces(std::string_view view)
		{
			using namespace lexer;
			std::size_t delta = 0;
			CommandArgs args;
			while (true)
			{
				auto iter = std::find_if(view.begin() + delta, view.end(), Alphabet::isWhitespace);
				if (iter != view.end())
				{
					auto temp = std::distance(view.begin(), iter);
					if(temp-delta != 0)
					{
						auto subArg = std::string(view.substr(delta, temp-delta));
						args.push_back(subArg);
						delta=temp+1;
					}
					else
					{
						delta++;
					}
				}
				else
				{
					args.push_back(std::string(view.substr(delta,view.size()-delta)));
					break;
				}
			}

			return args;
		}
	};
	class Driver
	{
		inline void emitErr(std::string_view errMsg)
		{
			CR_LOG_ERROR(errMsg);
			m_reStatus = BAD;
		}
	 public:
		enum ReStatus : int
		{
			GOOD = 0,
			BAD = 1
		};
		Driver(const CommandArgs& args)
		{
			greeting();
			argEmit(args);
		}
		Driver(int argc, char** argv)
		{
			greeting();
		}
		Driver()
		{
			greeting();
		}
		inline void greeting()
		{
			CR_LOG_INFO("Corrosion | Rust ASM Compiler made with C++ only for learning purposes\n ");
		}

		void mainArg(int argc, char** argv)
		{
			CommandArgs args;
			for(int i = 0; i < argc;i++)
			{
				args.push_back(argv[i]);
			}
			argEmit(args);
		}

		void argEmit(const CommandArgs& args)
		{
			if(args.size()==1)
			{
				auto&& [stmts,status] = m_parserInterface.getFastAst(args.back());
				if(!status)
				{
					emitErr("we find errors on previous stages, can't continue to parse");
				}
			}
			else
			{
				emitErr("for now compiler takes only one command line argument - file path to parse");
				return;
			}
		}
		ReStatus ret()
		{
			return m_reStatus;
		}

	 private:
		ReStatus m_reStatus = GOOD;
		ParserInterface m_parserInterface;

	};
}

#endif //CORROSION_SRC_DRIVER_DRIVER_HPP_
