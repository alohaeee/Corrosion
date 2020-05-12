#include "symbol.hpp"
namespace corrosion
{
	void Interner::fresh()
	{
		assert(s_strings.size() == 0 && s_names.size() == 0);
		// TODO: sort by hand later
		prefillLazy({
			// Special reserved identifiers used internally for elided lifetimes,
			// unnamed method parameters, crate root module, error recovery etc.
			{ "", kw::Invalid },
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
			{ "abstract", kw::Abstract },
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

			{ "bool", sym::Bool },
			{ "char", sym::Char },
			{ "f32", sym::F32 },
			{ "f64", sym::F64 },
			{ "i128", sym::I128 },
			{ "i16", sym::I16 },
			{ "i32", sym::I32 },
			{ "i64", sym::I64 },
			{ "i8", sym::I8 },
			{ "int", sym::Int },
			{ "isize", sym::ISize },
			{ "str", sym::Str },
			{ "u128", sym::U128 },
			{ "u16", sym::U16 },
			{ "u32", sym::U32 },
			{ "u64", sym::U64 },
			{ "u8", sym::U8 },
			{ "uint", sym::UInt },
			{ "usize", sym::USize },
		});
	}
	Ident::Ident(std::string_view view, Span span) : m_name{Interner::intern(view)}, m_span{span}
	{}
	std::string_view Symbol::toString() const
	{
		return Interner::get(*this);
	}
}
