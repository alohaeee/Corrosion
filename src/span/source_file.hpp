#ifndef CORROSION_SRC_SPAN_SOURCE_FILE_HPP_
#define CORROSION_SRC_SPAN_SOURCE_FILE_HPP_

#include "utility/std_incl.hpp"
#include "utility/file_loader.hpp"

namespace corrosion
{
	class SourceFile
	{
	 public:
		static std::vector<std::size_t> analiseSource(std::string_view view)
		{
			auto size = view.size();
			std::size_t last_index = 0;
			std::vector<std::size_t> lines;
			while(last_index < size)
			{

				auto iter = std::find(view.begin()+last_index,view.end(), '\n');
				last_index = std::distance(view.begin(), iter)+1;
				lines.push_back(last_index);
			}
			return lines;
		}
		SourceFile(const std::filesystem::path path, std::string_view str) : m_path(path), m_src(str)
		{
			m_lines = analiseSource(m_src);
		}
		SourceFile(const std::filesystem::path path) : m_path(path)
		{
			m_src = FileLoader::readFile(m_path);
			m_lines = analiseSource(m_src);
		}
		[[nodiscard]] std::string_view source() const
		{
			return m_src;
		}
		[[nodiscard]] const std::filesystem::path& path() const
		{
			return m_path;
		}
		[[nodiscard]] const std::vector<std::size_t>& lines() const
		{
			return m_lines;
		}
		[[nodiscard]] std::string_view getLine(std::size_t lineNumber)
		{
			if(lineNumber == 0)
			{

				return std::string_view(m_src).substr(0,m_lines[lineNumber]);
			}
			else if(lineNumber < m_lines.size())
			{
				return std::string_view(m_src).substr(m_lines[lineNumber-1], m_lines[lineNumber]-m_lines[lineNumber-1]);
			}
			throw std::out_of_range("Out of range in getLine method.");
		}
		[[nodiscard]] std::size_t lineCount() const
		{
			return m_lines.size();
		}
		[[nodiscard]] std::size_t inLine(std::size_t byte) const
		{
			auto result = std::lower_bound(m_lines.begin(), m_lines.end(),byte);
			if(result == m_lines.end())
			{
				return -1;
			}
			return std::distance(m_lines.begin(),result);
		}
		std::string_view fromSpan(Span span) const
		{
			return std::string_view(m_src).substr(span.lo(), span.hi()-span.lo());
		}
	 protected:
		/// The name of the file that the source came from.
		std::filesystem::path m_path;
		/// The complete source code.
		std::string m_src;
		/// Locations of lines beginnings in the source code.
		std::vector<std::size_t> m_lines;
	};
}

#endif //CORROSION_SRC_SPAN_SOURCE_FILE_HPP_
