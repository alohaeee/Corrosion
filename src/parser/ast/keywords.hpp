#ifndef CORROSION_SRC_PARSER_AST_KEYWORD_HPP_
#define CORROSION_SRC_PARSER_AST_KEYWORD_HPP_

namespace corrosion
{
//	enum KeywordsKind
//	{
//		// Special reserved identifiers used internally for elided lifetimes,
//		// unnamed method parameters, crate root module, error recovery etc.
//		Invalid,
//		PathRoot,
//		DollarCrate,
//		Underscore,
//		// Keywords that are used in stable Rust.
//		As,
//		Break,
//		Const,
//		Continue,
//		Crate,
//		Else,
//		Enum,
//		Extern,
//		False,
//		Fn,
//		For,
//		If,
//		Impl,
//		In,
//		Let,
//		Loop,
//		Match,
//		Mod,
//		Move,
//		Mut,
//		Pub,
//		Ref,
//		Return,
//		SelfLower,
//		SelfUpper,
//		Static,
//		Struct,
//		Super,
//		Trait,
//		True,
//		Type,
//		Unsafe,
//		Use,
//		Where,
//		While,
//		// Keywords that are used in unstable Rust or reserved for future use.
//		Abstract,
//		Become,
//		Box,
//		Do,
//		Final,
//		Macro,
//		Override,
//		Priv,
//		Typeof,
//		Unsized,
//		Virtual,
//		Yield,
//		// Edition-specific keywords that are used in stable Rust.
//		Async,
//		Await,
//		Dyn,
//		// Edition-specific keywords that are used in unstable Rust or reserved for future use.
//		Try,
//		// Special lifetime names
//		UnderscoreLifetime,
//		StaticLifetime,
//		// Weak keywords, have special meaning only in specific contexts.
//		Auto,
//		Catch,
//		Default,
//		MacroRules,
//		Raw,
//		Union
//	};
//	struct Keywords
//	{
//		static inline const std::unordered_map<std::string, std::uint32_t> dictionaryStrKeyword
//			{
//				// Special reserved identifiers used internally for elided lifetimes,
//				// unnamed method parameters, crate root module, error recovery etc.
//				//{ "{{root}}", KeywordKind::PathRoot, },
//				{ "$crate", kw::DollarCrate, },
//				{ "_", kw::Underscore },
//
//				// Keywords that are used in stable Rust.
//				{ "as", kw::As },
//				{ "break", kw::Break },
//				{ "const", kw::Const },
//				{ "continue", kw::Continue },
//				{ "crate", kw::Crate, },
//				{ "else", kw::Else },
//				{ "enum", kw::Enum },
//				{ "extern", kw::Extern },
//				{ "false", kw::False },
//				{ "fn", kw::Fn },
//				{ "for", kw::For },
//				{ "if", kw::If },
//				{ "impl", kw::Impl },
//				{ "in", kw::In },
//				{ "let", kw::Let },
//				{ "loop", kw::Loop },
//				{ "match", kw::Match },
//				{ "mod", kw::Mod },
//				{ "move", kw::Move },
//				{ "mut", kw::Mut },
//				{ "pub", kw::Pub },
//				{ "ref", kw::Ref },
//				{ "return", kw::Return },
//				{ "self", kw::SelfLower },
//				{ "Self", kw::SelfUpper },
//				{ "static", kw::Static },
//				{ "struct", kw::Struct },
//				{ "super", kw::Super },
//				{ "trait", kw::Trait },
//				{ "true", kw::True },
//				{ "type", kw::Type },
//				{ "unsafe", kw::Unsafe },
//				{ "use", kw::Use },
//				{ "where", kw::Where },
//				{ "while", kw::While },
//
//				// Keywords that are used in unstable Rust or reserved for future use.
//				{ "become", kw::Become },
//				{ "box", kw::Box },
//				{ "do", kw::Do },
//				{ "final", kw::Final },
//				{ "macro", kw::Macro },
//				{ "override", kw::Override },
//				{ "priv", kw::Priv },
//				{ "typeof", kw::Typeof },
//				{ "unsized", kw::Unsized },
//				{ "virtual", kw::Virtual },
//				{ "yield", kw::Yield },
//
//				// Edition-specific keywords that are used in stable Rust.
//				{ "async", kw::Async }, // >= 2018 Edition only
//				{ "await", kw::Await }, // >= 2018 Edition only
//				{ "dyn", kw::Dyn }, // >= 2018 Edition only
//
//				// Edition-specific keywords that are used in unstable Rust or reserved for future use.
//				{ "try", kw::Try }, // >= 2018 Edition only
//
//				// Special lifetime names  names
//				{ "'_", kw::UnderscoreLifetime },
//				{ "'static", kw::StaticLifetime },
//
//				// Weak keywords, have special meaning only in specific contexts. ave special meaning only in specific contexts.
//				{ "auto", kw::Auto },
//				{ "catch", kw::Catch },
//				{ "default", kw::Default },
//				{ "macro_rules", kw::MacroRules },
//				{ "raw", kw::Raw },
//				{ "union", kw::Union }
//			};
//		static inline const std::vector<std::pair<std::string, std::uint32_t>,32> dictionary
//			{
//				// Special reserved identifiers used internally for elided lifetimes,
//				// unnamed method parameters, crate root module, error recovery etc.
//				//{ "{{root}}", KeywordKind::PathRoot, },
//				{ "$crate", kw::DollarCrate, },
//				{ "_", kw::Underscore },
//
//				// Keywords that are used in stable Rust.
//				{ "as", kw::As },
//				{ "break", kw::Break },
//				{ "const", kw::Const },
//				{ "continue", kw::Continue },
//				{ "crate", kw::Crate, },
//				{ "else", kw::Else },
//				{ "enum", kw::Enum },
//				{ "extern", kw::Extern },
//				{ "false", kw::False },
//				{ "fn", kw::Fn },
//				{ "for", kw::For },
//				{ "if", kw::If },
//				{ "impl", kw::Impl },
//				{ "in", kw::In },
//				{ "let", kw::Let },
//				{ "loop", kw::Loop },
//				{ "match", kw::Match },
//				{ "mod", kw::Mod },
//				{ "move", kw::Move },
//				{ "mut", kw::Mut },
//				{ "pub", kw::Pub },
//				{ "ref", kw::Ref },
//				{ "return", kw::Return },
//				{ "self", kw::SelfLower },
//				{ "Self", kw::SelfUpper },
//				{ "static", kw::Static },
//				{ "struct", kw::Struct },
//				{ "super", kw::Super },
//				{ "trait", kw::Trait },
//				{ "true", kw::True },
//				{ "type", kw::Type },
//				{ "unsafe", kw::Unsafe },
//				{ "use", kw::Use },
//				{ "where", kw::Where },
//				{ "while", kw::While },
//
//				// Keywords that are used in unstable Rust or reserved for future use.
//				{ "become", kw::Become },
//				{ "box", kw::Box },
//				{ "do", kw::Do },
//				{ "final", kw::Final },
//				{ "macro", kw::Macro },
//				{ "override", kw::Override },
//				{ "priv", kw::Priv },
//				{ "typeof", kw::Typeof },
//				{ "unsized", kw::Unsized },
//				{ "virtual", kw::Virtual },
//				{ "yield", kw::Yield },
//
//				// Edition-specific keywords that are used in stable Rust.
//				{ "async", kw::Async }, // >= 2018 Edition only
//				{ "await", kw::Await }, // >= 2018 Edition only
//				{ "dyn", kw::Dyn }, // >= 2018 Edition only
//
//				// Edition-specific keywords that are used in unstable Rust or reserved for future use.
//				{ "try", kw::Try }, // >= 2018 Edition only
//
//				// Special lifetime names  names
//				{ "'_", kw::UnderscoreLifetime },
//				{ "'static", kw::StaticLifetime },
//
//				// Weak keywords, have special meaning only in specific contexts. ave special meaning only in specific contexts.
//				{ "auto", kw::Auto },
//				{ "catch", kw::Catch },
//				{ "default", kw::Default },
//				{ "macro_rules", kw::MacroRules },
//				{ "raw", kw::Raw },
//				{ "union", kw::Union }
//			};


//		static KeywordKind keywordFromStr(std::string_view view)
//		{
//			auto res = dictionaryStrKeyword.find(view.data());
//			if(res == dictionaryStrKeyword.end())
//			{
//				return KeywordKind::Invalid;
//			}
//			return res->second;
//		}
//		static std::string toString(KeywordKind keyword)
//		{
////			auto res = dictionaryKeywordStr.find(keyword);
////			if(res == dictionaryKeywordStr.end())
////			{
////				return "";
////			}
////			return res->second;
//		}
//		static inline bool isKeyword(KeywordKind kind)
//		{
//			return kind != KeywordKind::Invalid;
//		}
//		static inline bool isKeyword(std::string_view view)
//		{
//			return isKeyword(keywordFromStr(view));
//		}

}

#endif //CORROSION_SRC_PARSER_AST_KEYWORD_HPP_
