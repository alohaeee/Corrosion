#ifndef CORROSION_SRC_PARSER_INTERFACE_HPP_
#define CORROSION_SRC_PARSER_INTERFACE_HPP_

#include "parser.hpp"
#include "lexer/string_reader.hpp"
#include "lexer/tokentrees.hpp"

namespace corrosion
{
	struct ParserInterface
	{
		ParserInterface()
		{
			if (s_init)
			{
				return;
			}
			else
			{
				Log::init();
				Interner::fresh();
				s_init= true;
			}
		}
		std::pair<std::vector<Stmt>, bool> getFastAst(const std::filesystem::path& path)
		{
			std::vector<Stmt> stmts;
			try
			{
				auto parseSession = std::make_shared<ParseSession>();
				parseSession->appendFile(SourceFile(path));
				StringReader reader{ parseSession };
				TokenTreeReader treeReader{ std::move(reader) };
				auto stream = treeReader.parseAllTokenTrees();

				Parser parser{ parseSession, std::move(stream) };

				try
				{
					while (true)
					{
						if (auto stmt = parser.parseFullStmt();stmt)
						{
							stmts.push_back(std::move(stmt.value()));
						}
						else
						{
							break;
						}
					}
				}
				catch ([[maybe_unused]]CriticalException& e)
				{
					parser.printAst(stmts);
					throw e;
				}
				catch (std::exception& e)
				{
					parser.printAst(stmts);
					throw e;
				}
				parser.printAst(stmts);
				if(parseSession->errorHandler().errorCount())
				{
					return {stmts,false};
				}
				else
				{
					return {stmts,true};
				}

			}

			catch ([[maybe_unused]]CriticalException& e)
			{
				//CR_LOG_CRITICAL(e.what());
				return {{},false};
			}
			catch (std::exception& e)
			{
				CR_LOG_CRITICAL(e.what());
				return {{},false};
			}

		}
		static bool inited()
		{
			return s_init;
		}
	 private:
		static inline bool s_init = false;
	};
}

#endif //CORROSION_SRC_PARSER_INTERFACE_HPP_
