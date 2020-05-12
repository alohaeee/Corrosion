#ifndef CORROSION_SRC_SPAN_SYMBOL_HPP_
#define CORROSION_SRC_SPAN_SYMBOL_HPP_

#include "utility/fwd.hpp"

#include "span/keywords_and_syms.hpp"
#include "span/span.hpp"

namespace corrosion
{

	class Interner;
	class Symbol
	{
	 public:

		using index_type = std::uint32_t;
		Symbol(std::uint32_t n) : m_id(n)
		{
		}
		static Symbol intern(std::string_view view)
		{
			return { 0 };
		}
		Symbol(kw keyword) : m_id(static_cast<std::uint32_t>(keyword))
		{
		}
		Symbol(sym keyword) : m_id(static_cast<std::uint32_t>(keyword))
		{
		}

		std::string_view toString() const;

		bool isPathSegment() const noexcept
		{
			return *this == kw::Super || *this == kw::SelfLower || *this == kw::Crate ||
				*this == kw::PathRoot || *this == kw::DollarCrate;
		}
		bool isBoolLiteral() const noexcept
		{
			return *this == kw::True || *this == kw::False;
		}
		bool canBeRawIdent() const noexcept
		{
			return *this != kw::Invalid && *this != kw::Underscore && !isPathSegment();
		}

		friend bool operator<(Symbol lhv, Symbol rhv)
		{
			return lhv.m_id < rhv.m_id;
		}
		friend bool operator>(Symbol lhv, Symbol rhv)
		{
			return lhv.m_id > rhv.m_id;
		}
		friend bool operator<=(Symbol lhv, Symbol rhv)
		{
			return lhv.m_id <= rhv.m_id;
		}
		friend bool operator>=(Symbol lhv, Symbol rhv)
		{
			return lhv.m_id >= rhv.m_id;
		}
		friend bool operator==(Symbol lhv, Symbol rhv)
		{
			return lhv.m_id == rhv.m_id;
		}
		friend bool operator!=(Symbol lhv, Symbol rhv)
		{
			return lhv.m_id != rhv.m_id;
		}

		[[nodiscard]] const std::uint32_t data() const
		{
			return m_id;
		}
	 protected:
		std::uint32_t m_id;
	};
	class Ident
	{
	 public:
		Ident(Symbol name = { kw::Invalid }, Span span = {}) : m_name{ name }, m_span{ span }
		{
		}
		Ident(std::string_view view, Span span = {});

		Symbol name() const noexcept
		{
			return m_name;
		}
		Span span() const noexcept
		{
			return m_span;
		}

		// Returns `true` for reserved identifiers used internally for elided lifetimes,
		// unnamed method parameters, crate root module, error recovery etc.
		bool isSpecial() const noexcept
		{
			return m_name <= kw::Underscore;
		}
		/// Returns `true` if the token is a keyword used in the language.
		bool isUsedKeyword() const noexcept
		{
			return m_name >= kw::As && m_name <=kw::Yield;
		}

	 private:
		Symbol m_name;
		Span m_span;

	};
	class Interner
	{
	 public:
		[[nodiscard]] static Symbol intern(std::string_view view)
		{
			if (auto iter = s_names.find(view); iter != s_names.end())
			{
				return iter->second;
			}

			s_strings.push_back(view.data());
			auto inserted = s_names.emplace(s_strings.back(), Symbol{ s_strings.size() });

			return inserted.second;
		}
		[[nodiscard]] inline static std::string_view get(Symbol symbol)
		{
			return s_strings[symbol.data()];
		}

		static void prefill(const std::vector<std::pair<std::string, Symbol>>&& data)
		{
			s_strings.reserve(data.size());
			s_names.reserve(data.size());

			for (auto &&[str, sym]:data)
			{
				s_strings.push_back(str);
				s_names.emplace(s_strings.back(), sym);
			}
		}
		static void prefillLazy(std::vector<std::pair<std::string, Symbol>>&& data)
		{
			std::sort(data.begin(), data.end(), [](auto& a, auto& b)
			{
			  return a.second <= b.second;
			});
			prefill(std::move(data));
		}
		//! init
		static void fresh();

		static const auto& names()
		{
			return s_names;
		}
		static const auto& strings()
		{
			return s_strings;
		}

	 private:
		inline static std::unordered_map<std::string_view, Symbol> s_names{};
		inline static std::vector<std::string> s_strings{};
	};

}

#endif //CORROSION_SRC_SPAN_SYMBOL_HPP_
