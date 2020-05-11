#ifndef CORROSION_SRC_SPAN_SYMBOL_HPP_
#define CORROSION_SRC_SPAN_SYMBOL_HPP_

#include "utility/fwd.hpp"

#include "span/keywords_and_syms.hpp"

namespace corrosion
{

	class Symbol
	{
	 public:

		Symbol(std::uint32_t n) : m_index(n)
		{
		}
		static Symbol intern(std::string_view view)
		{
			return { 0 };
		}
		Symbol(kw keyword) : m_index(static_cast<std::uint32_t>(keyword))
		{}
		Symbol(sym keyword) : m_index(static_cast<std::uint32_t>(keyword))
		{}
		friend bool operator<(Symbol lhv, Symbol rhv)
		{
			return lhv.m_index < rhv.m_index;
		}
		friend bool operator>(Symbol lhv, Symbol rhv)
		{
			return lhv.m_index > rhv.m_index;
		}
		friend bool operator<=(Symbol lhv, Symbol rhv)
		{
			return lhv.m_index <= rhv.m_index;
		}
		friend bool operator>=(Symbol lhv, Symbol rhv)
		{
			return lhv.m_index >= rhv.m_index;
		}
		friend bool operator==(Symbol lhv, Symbol rhv)
		{
			return lhv.m_index == rhv.m_index;
		}
		friend bool operator!=(Symbol lhv, Symbol rhv)
		{
			return lhv.m_index != rhv.m_index;
		}


		[[nodiscard]] const std::uint32_t data() const
		{
			return m_index;
		}
	 protected:
		std::uint32_t m_index;
	};
	struct Interner
	{
	 public:
		[[nodiscard]] Symbol intern(std::string_view view)
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

		static void prefill(const std::vector<std::pair<std::string,Symbol>>&& data)
		{
			s_strings.reserve(data.size());
			s_names.reserve(data.size());

			for(auto &&[str,sym]:data)
			{
				s_strings.push_back(str);
				s_names.emplace(s_strings.back(), sym);
			}
		}
		static void prefillLazy(std::vector<std::pair<std::string,Symbol>>&& data)
		{
			std::sort(data.begin(),data.end(), [](auto& a, auto& b)
			{
			  return a.second <= b.second;
			});
			prefill(std::move(data));
		}
		// init
		static void fresh()
		{
			assert(s_strings.size() == 0 && s_names.size() == 0);
			prefillLazy({
				// Special reserved identifiers used internally for elided lifetimes,
				// unnamed method parameters, crate root module, error recovery etc.
				{ "", kw::Invalid},
				{ "{{root}}", kw::PathRoot, },
				{ "$crate", kw::DollarCrate, },
				{ "_", kw::Underscore },

				// Keywords that are used in stable Rust.
				{ "as", kw::As },
				{ "break", kw::Break },
				{ "const", kw::Const },
				{ "continue", kw::Continue },
				{ "crate", kw::Crate, },
				{ "else", kw::Else },
				{ "enum", kw::Enum },
				{ "extern", kw::Extern },
				{ "false", kw::False },
				{ "fn", kw::Fn },
				{ "for", kw::For },
				{ "if", kw::If },
				{ "impl", kw::Impl },
				{ "in", kw::In },
				{ "let", kw::Let },
				{ "loop", kw::Loop },
				{ "match", kw::Match },
				{ "mod", kw::Mod },
				{ "move", kw::Move },
				{ "mut", kw::Mut },
				{ "pub", kw::Pub },
				{ "ref", kw::Ref },
				{ "return", kw::Return },
				{ "self", kw::SelfLower },
				{ "Self", kw::SelfUpper },
				{ "static", kw::Static },
				{ "struct", kw::Struct },
				{ "super", kw::Super },
				{ "trait", kw::Trait },
				{ "true", kw::True },
				{ "type", kw::Type },
				{ "unsafe", kw::Unsafe },
				{ "use", kw::Use },
				{ "where", kw::Where },
				{ "while", kw::While },

				// Keywords that are used in unstable Rust or reserved for future use.
				{"abstract", kw::Abstract},
				{ "become", kw::Become },
				{ "box", kw::Box },
				{ "do", kw::Do },
				{ "final", kw::Final },
				{ "macro", kw::Macro },
				{ "override", kw::Override },
				{ "priv", kw::Priv },
				{ "typeof", kw::Typeof },
				{ "unsized", kw::Unsized },
				{ "virtual", kw::Virtual },
				{ "yield", kw::Yield },

				// Edition-specific keywords that are used in stable Rust.
				{ "async", kw::Async }, // >= 2018 Edition only
				{ "await", kw::Await }, // >= 2018 Edition only
				{ "dyn", kw::Dyn }, // >= 2018 Edition only

				// Edition-specific keywords that are used in unstable Rust or reserved for future use.
				{ "try", kw::Try }, // >= 2018 Edition only

				// Special lifetime names  names
				{ "'_", kw::UnderscoreLifetime },
				{ "'static", kw::StaticLifetime },

				// Weak keywords, have special meaning only in specific contexts. ave special meaning only in specific contexts.
				{ "auto", kw::Auto },
				{ "catch", kw::Catch },
				{ "default", kw::Default },
				{ "macro_rules", kw::MacroRules },
				{ "raw", kw::Raw },
				{ "union", kw::Union },

				{"bool", sym::Bool},
				{"char", sym::Char},
				{"f32", sym::F32},
				{"f64", sym::F64},
				{"i128", sym::I128},
				{"i16", sym::I16},
				{"i32", sym::I32},
				{"i64", sym::I64},
				{"i8", sym::I8},
				{"int", sym::Int},
				{"isize", sym::ISize},
				{"str", sym::Str},
				{"u128", sym::U128},
				{"u16", sym::U16},
				{"u32", sym::U32},
				{"u64", sym::U64},
				{"u8",  sym::U8},
				{"uint", sym::UInt},
				{"usize", sym::USize},
			});
		}


		inline static std::unordered_map<std::string_view, Symbol> s_names{};
		inline static std::vector<std::string> s_strings{};
	};

}

#endif //CORROSION_SRC_SPAN_SYMBOL_HPP_
