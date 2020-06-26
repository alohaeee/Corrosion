#ifndef CORROSION_SRC_ERROR_ERROR_HPP_
#define CORROSION_SRC_ERROR_ERROR_HPP_

#include "utility/std_incl.hpp"
#include "error/log.hpp"
#include <exception>
#include "span/span.hpp"

namespace corrosion
{

	class ErrorHandler
	{
	 public:
		class CriticalException: public std::exception
		{
		 public:
			CriticalException(std::string_view msg) : std::exception(msg.data())
			{}
		};
		ErrorHandler& info(std::string_view msg) noexcept
		{
			CR_LOG_INFO(msg);
			return *this;
		}
		ErrorHandler& warn(std::string_view msg) noexcept
		{
			CR_LOG_WARN(msg);
			return *this;
		}
		ErrorHandler& error(std::string_view msg) noexcept
		{
			m_errorCount++;
			CR_LOG_ERROR(msg);
			return *this;
		}
		void critical(std::string_view msg)
		{
			m_errorCount++;
			CR_LOG_CRITICAL(msg);
			throw CriticalException(msg);
		}
		ErrorHandler& trace(std::string_view msg) noexcept
		{
			CR_LOG_TRACE(msg);
			return *this;
		}
		std::uint32_t errorCount() noexcept
		{
			return m_errorCount;
		}
	 protected:
		std::uint32_t m_errorCount = 0;
	};
}

#endif //CORROSION_SRC_ERROR_ERROR_HPP_
