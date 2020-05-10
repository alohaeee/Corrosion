//
// Created by Vladislav on 05.05.2020.
//
#ifndef CORROSION_SRC_PARSER_SPAN_SPAN_HPP_
#define CORROSION_SRC_PARSER_SPAN_SPAN_HPP_

namespace corrosion
{
	struct Span
	{
		Span(std::uint32_t lo=0, std::uint32_t hi=0)
		{
			if (lo > hi)
			{
				std::swap(lo, hi);
			}
			m_lo = lo;
			m_hi = hi;
		}
		[[nodiscard]] std::uint32_t len() const noexcept
		{
			return m_hi - m_lo;
		}
		[[nodiscard]] bool isDummy() const noexcept
		{
			return !m_lo && !m_hi;
		}
		[[nodiscard]] std::uint32_t lo() const noexcept
		{
			return m_lo;
		}
		[[nodiscard]] std::uint32_t hi() const noexcept
		{
			return m_hi;
		};
		[[nodiscard]] std::pair<std::uint32_t, std::uint32_t> pair() const noexcept
		{
			return { m_lo, m_hi };
		}

		/// @brief Returns `true` if `self` fully encloses `other`.
		[[nodiscard]] bool contains(Span other) const noexcept
		{
			return m_lo >= other.m_lo && other.m_hi <= m_hi;
		}

		/// @brief Returns `true` if `self` touches `other`.
		[[nodiscard]] bool overlaps(Span other) const noexcept
		{
			return m_lo > other.m_lo || other.m_hi < m_hi;
		}

		[[nodiscard]] bool operator==(Span other) const noexcept
		{
			return m_lo == other.m_lo && m_hi == other.m_hi;
		}
		[[nodiscard]] bool operator!=(Span other) const noexcept
		{
			return !(*this == other);
		}

		[[nodiscard]] static Span sum(Span lhv, Span rhv) noexcept
		{
			auto lo = std::max(lhv.m_lo, rhv.m_lo);
			auto hi = std::max(lhv.m_hi, rhv.m_hi);

			return { lo, hi };
		}

	 protected:
		std::uint32_t m_lo;
		std::uint32_t m_hi;
	};

}
#endif //CORROSION_SRC_PARSER_SPAN_SPAN_HPP_
