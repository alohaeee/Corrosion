#ifndef CORROSION_SRC_PARSER_SPAN_SOURCE_MAP_HPP_
#define CORROSION_SRC_PARSER_SPAN_SOURCE_MAP_HPP_

#include "utility/std_incl.hpp"
#include "error/error.hpp"
#include "span/source_file.hpp"
#include "span/span.hpp"

namespace corrosion
{
	class ParseSession
	{
	 public:
		[[nodiscard]] const SourceFile& file() const noexcept
		{
			return m_file;
		}
		[[nodiscard]] const ErrorHandler& errorHandler() const noexcept
		{
			return m_error;
		}
		void appendFile(SourceFile&& file)
		{
			m_file = file;
		}

		std::string emitSpan(Span span)
		{
			if(span.isDummy())
			{
				return "";
			}
			auto lo_line = m_file.inLine(span.lo());
			auto hi_line = m_file.inLine(span.hi());
			auto hi_str_size = std::to_string(hi_line).size();

			std::string source{ fmt::format("{0:>{1}}", " | \n", hi_str_size+1) };
			for (std::size_t cur_line = lo_line; cur_line <= hi_line; cur_line++)
			{
				auto line = m_file.getLine(cur_line);
				if(line.back() == '\n')
				{
					line = line.substr(0,line.size()-1);
				}
				source.append(fmt::format("{0:<{1}}| {2}\n", cur_line, hi_str_size, line));
			}
			// create span pointer "^^^^"
			std::string span_pointer;
			span_pointer.resize(span.hi()- span.lo());
			std::fill(span_pointer.begin(),span_pointer.end(),'^');
			// "-----"
			auto delta = 0;
			if(lo_line != 0)
			{
				delta = m_file.lines()[lo_line-1];
			}
			std::size_t pre_render = span.lo() - delta;

			source.append( fmt::format("{0:>{1}}", " | ", hi_str_size+1));
			source.append(fmt::format("{0:->{1}}", span_pointer, pre_render+span_pointer.size()));
			return source;
		}

		ParseSession& warn(std::string_view msg) noexcept
		{
			m_error.warn(msg);
			return *this;
		}
		ParseSession& error(std::string_view msg) noexcept
		{
			m_error.error(msg);
			return *this;
		}
		ParseSession& info(std::string_view msg) noexcept
		{
			m_error.info(msg);
			return *this;
		}
		void critical(std::string_view msg)
		{
			m_error.critical(msg);
		}
		ParseSession& trace(std::string_view msg) noexcept
		{
			m_error.trace(msg);
			return *this;
		}
		ParseSession& warnSpan(Span span,std::string_view msg) noexcept
		{
			auto spanned = this->emitSpan(span);
			m_error.warn(fmt::format("{}\n--->{}\n{}\n",msg, m_file.path().filename().string(),spanned));
			return *this;
		}
		ParseSession& errorSpan(Span span,std::string_view msg) noexcept
		{
			auto spanned = this->emitSpan(span);
			m_error.error(fmt::format("{}\n--->{}\n{}\n",msg, m_file.path().filename().string(),spanned));
			return *this;
		}
		void criticalSpan(Span span, std::string_view msg)
		{
			auto spanned = this->emitSpan(span);
			m_error.critical(fmt::format("{}\n--->{}\n{}\n",msg, m_file.path().filename().string(),spanned));
		}
		ParseSession& traceSpan(Span span, std::string_view msg) noexcept
		{
			auto spanned = this->emitSpan(span);
			m_error.trace(fmt::format("{}\n--->{}\n{}\n",msg, m_file.path().filename().string(),spanned));
			return *this;
		}
		ParseSession& infoSpan(Span span, std::string_view msg) noexcept
		{
			auto spanned = this->emitSpan(span);
			m_error.info(fmt::format("{}\n--->{}\n{}\n",msg, m_file.path().filename().string(),spanned));
			return *this;
		}

	 protected:
		ErrorHandler m_error{};
		SourceFile m_file{ "", "" };

	};
}

#endif //CORROSION_SRC_PARSER_SPAN_SOURCE_MAP_HPP_
