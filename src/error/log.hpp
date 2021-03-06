#ifndef CORROSION_SRC_LOG_HPP_
#define CORROSION_SRC_LOG_HPP_

#include "utility/std_incl.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#ifdef CR_ENABLE_DEBUG_LOG_ALL
#define CR_ENABLE_DEBUG_LOG
#define CR_ENABLE_LOG_TOKENIZER
#define CR_ENABLE_LOG_STRREADER
#define CR_ENABLE_LOG_AST
#endif

namespace corrosion
{
	class Log
	{
	 public:
		static void init()
		{
			s_parseLogger = spdlog::stdout_color_mt("Corrosion");
			s_parseLogger->set_level(spdlog::level::trace);
			s_parseLogger->set_pattern("%^%l:%$ %v");

#ifdef CR_ENABLE_DEBUG_LOG
			s_parseLogger->info("Tokenizer will makes log output into 'logs/tokenizer.txt'");
			FileLoader::create(std::filesystem::current_path()/"logs"/"debug.txt");
			s_debugLogger = spdlog::basic_logger_mt("Debug",
				(std::filesystem::current_path()/"logs"/"debug.txt").string());
			s_debugLogger->set_level(spdlog::level::trace);
			s_debugLogger->set_pattern("%^[%T] <%n>%$ %v");
#endif

#ifdef CR_ENABLE_LOG_TOKENIZER
			s_parseLogger->info("Tokenizer will makes log output into 'logs/tokenizer.txt'");
			FileLoader::create(std::filesystem::current_path()/"logs"/"tokenizer.txt");
			s_tokenizerLogger = spdlog::basic_logger_mt("Tokenizer",
				(std::filesystem::current_path()/"logs"/"tokenizer.txt").string());
			s_tokenizerLogger->set_level(spdlog::level::trace);
			s_tokenizerLogger->set_pattern("%^[%T] [%n] %v%$");
#endif
#ifdef CR_ENABLE_LOG_STRREADER
			s_parseLogger->info("StringReader and TreeReader will makes log output into 'logs/string_reader.txt'");
			FileLoader::create(std::filesystem::current_path()/"logs"/"string_reader.txt");
			s_stringReaderLogger = spdlog::basic_logger_mt("StringReader",
				(std::filesystem::current_path()/"logs"/"string_reader.txt").string());
			s_stringReaderLogger->set_level(spdlog::level::trace);
			s_stringReaderLogger->set_pattern("%v%$");
#endif
#ifdef CR_ENABLE_LOG_AST
			s_parseLogger->info("Parser will makes log output into 'logs/ast.txt'");
			FileLoader::create(std::filesystem::current_path()/"logs"/"ast.txt");
			s_astLogger = spdlog::basic_logger_mt("AST",
				(std::filesystem::current_path()/"logs"/"ast.txt").string());
			s_astLogger->set_level(spdlog::level::trace);
			s_astLogger->set_pattern("%v%$");
#endif

		}
		inline static std::shared_ptr<spdlog::logger>& getParseLogger()
		{
			return s_parseLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& getDebugLogger()
		{
			return s_debugLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& getTokenizerLogger()
		{
			return s_tokenizerLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& getStringReaderLogger()
		{
			return s_stringReaderLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& getAstLogger()
		{
			return s_astLogger;
		}
	 protected:
		inline static std::shared_ptr<spdlog::logger> s_parseLogger;
		inline static std::shared_ptr<spdlog::logger> s_debugLogger;
		inline static std::shared_ptr<spdlog::logger> s_tokenizerLogger;
		inline static std::shared_ptr<spdlog::logger> s_stringReaderLogger;
		inline static std::shared_ptr<spdlog::logger> s_astLogger;
	};
#define CR_LOG_TRACE(...) ::corrosion::Log::getParseLogger()->trace(__VA_ARGS__)
#define CR_LOG_INFO(...) ::corrosion::Log::getParseLogger()->info(__VA_ARGS__)
#define CR_LOG_WARN(...) ::corrosion::Log::getParseLogger()->warn(__VA_ARGS__)
#define CR_LOG_ERROR(...) ::corrosion::Log::getParseLogger()->error(__VA_ARGS__)
#define CR_LOG_CRITICAL(...) ::corrosion::Log::getParseLogger()->critical(__VA_ARGS__)
#define CR_LOG_DEBUG(...) ::corrosion::Log::getParseLogger()->debug(__VA_ARGS__)


#ifdef CR_ENABLE_DEBUG_LOG
#define CR_DEBUG_LOG_ERROR(...) ::corrosion::Log::getDebugLogger()->error(__VA_ARGS__)
#define CR_DEBUG_LOG_WARN(...) ::corrosion::Log::getDebugLogger()->warn(__VA_ARGS__)
#define CR_DEBUG_LOG_INFO(...) ::corrosion::Log::getDebugLogger()->info(__VA_ARGS__)
#define CR_DEBUG_LOG_TRACE(...) ::corrosion::Log::getDebugLogger()->trace(__VA_ARGS__); \
	::corrosion::Log::getDebugLogger()->flush()
#define CR_DEBUG_LOG_CRITICAL(...) ::corrosion::Log::getDebugLogger()->critical(__VA_ARGS__)
#else
#define CR_DEBUG_LOG_ERROR(...)
#define CR_DEBUG_LOG_WARN(...)
#define CR_DEBUG_LOG_INFO(...)
#define CR_DEBUG_LOG_TRACE(...)
#define CR_DEBUG_LOG_FATAL(...)
#define CR_DEBUG_LOG_CRITICAL(...)
#endif

#ifdef CR_ENABLE_LOG_TOKENIZER
#define CR_LOG_TOKENIZER(...) ::corrosion::Log::getTokenizerLogger()->trace(__VA_ARGS__)
#else
#define CR_LOG_TOKENIZER(...)
#endif

#ifdef CR_ENABLE_LOG_STRREADER
#define CR_LOG_STRREADER(...) ::corrosion::Log::getStringReaderLogger()->trace(__VA_ARGS__)
#else
#define CR_LOG_STRREADER(...)
#endif

#ifdef CR_ENABLE_LOG_AST
#define CR_LOG_AST(...) ::corrosion::Log::getAstLogger()->trace(__VA_ARGS__)
#else
#define CR_LOG_AST(...)
#endif
}

#endif  //CORROSION_SRC_LOG_HPP_
