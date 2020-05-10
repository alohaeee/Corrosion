#ifndef CORROSION_SRC_PARSER_AST_KEYWORD_HPP_
#define CORROSION_SRC_PARSER_AST_KEYWORD_HPP_

namespace corrosion
{
	enum class KeywordKind
	{
		// Special reserved identifiers used internally for elided lifetimes,
		// unnamed method parameters, crate root module, error recovery etc.
		Invalid,
		PathRoot,
		DollarCrate,
		Underscore,
		// Keywords that are used in stable Rust.
		As,
		Break,
		Const,
		Continue,
		Crate,
		Else,
		Enum,
		Extern,
		False,
		Fn,
		For,
		If,
		Impl,
		In,
		Let,
		Loop,
		Match,
		Mod,
		Move,
		Mut,
		Pub,
		Ref,
		Return,
		SelfLower,
		SelfUpper,
		Static,
		Struct,
		Super,
		Trait,
		True,
		Type,
		Unsafe,
		Use,
		Where,
		While,
		// Keywords that are used in unstable Rust or reserved for future use.
		Abstract,
		Become,
		Box,
		Do,
		Final,
		Macro,
		Override,
		Priv,
		Typeof,
		Unsized,
		Virtual,
		Yield,
		// Edition-specific keywords that are used in stable Rust.
		Async,
		Await,
		Dyn,
		// Edition-specific keywords that are used in unstable Rust or reserved for future use.
		Try,
		// Special lifetime names
		UnderscoreLifetime,
		StaticLifetime,
		// Weak keywords, have special meaning only in specific contexts.
		Auto,
		Catch,
		Default,
		MacroRules,
		Raw,
		Union
	};
	struct Keywords
	{
		static inline const std::unordered_map<std::string, KeywordKind> dictionaryStrKeyword
			{
				// Special reserved identifiers used internally for elided lifetimes,
				// unnamed method parameters, crate root module, error recovery etc.
				//{ "{{root}}", KeywordKind::PathRoot, },
				{ "$crate", KeywordKind::DollarCrate, },
				{ "_", KeywordKind::Underscore },

				// Keywords that are used in stable Rust.
				{ "as", KeywordKind::As },
				{ "break", KeywordKind::Break },
				{ "const", KeywordKind::Const },
				{ "continue", KeywordKind::Continue },
				{ "crate", KeywordKind::Crate, },
				{ "else", KeywordKind::Else },
				{ "enum", KeywordKind::Enum },
				{ "extern", KeywordKind::Extern },
				{ "false", KeywordKind::False },
				{ "fn", KeywordKind::Fn },
				{ "for", KeywordKind::For },
				{ "if", KeywordKind::If },
				{ "impl", KeywordKind::Impl },
				{ "in", KeywordKind::In },
				{ "let", KeywordKind::Let },
				{ "loop", KeywordKind::Loop },
				{ "match", KeywordKind::Match },
				{ "mod", KeywordKind::Mod },
				{ "move", KeywordKind::Move },
				{ "mut", KeywordKind::Mut },
				{ "pub", KeywordKind::Pub },
				{ "ref", KeywordKind::Ref },
				{ "return", KeywordKind::Return },
				{ "self", KeywordKind::SelfLower },
				{ "Self", KeywordKind::SelfUpper },
				{ "static", KeywordKind::Static },
				{ "struct", KeywordKind::Struct },
				{ "super", KeywordKind::Super },
				{ "trait", KeywordKind::Trait },
				{ "true", KeywordKind::True },
				{ "type", KeywordKind::Type },
				{ "unsafe", KeywordKind::Unsafe },
				{ "use", KeywordKind::Use },
				{ "where", KeywordKind::Where },
				{ "while", KeywordKind::While },

				// Keywords that are used in unstable Rust or reserved for future use.
				{ "become", KeywordKind::Become },
				{ "box", KeywordKind::Box },
				{ "do", KeywordKind::Do },
				{ "final", KeywordKind::Final },
				{ "macro", KeywordKind::Macro },
				{ "override", KeywordKind::Override },
				{ "priv", KeywordKind::Priv },
				{ "typeof", KeywordKind::Typeof },
				{ "unsized", KeywordKind::Unsized },
				{ "virtual", KeywordKind::Virtual },
				{ "yield", KeywordKind::Yield },

				// Edition-specific keywords that are used in stable Rust.
				{ "async", KeywordKind::Async }, // >= 2018 Edition only
				{ "await", KeywordKind::Await }, // >= 2018 Edition only
				{ "dyn", KeywordKind::Dyn }, // >= 2018 Edition only

				// Edition-specific keywords that are used in unstable Rust or reserved for future use.
				{ "try", KeywordKind::Try }, // >= 2018 Edition only

				// Special lifetime names  names
				{ "'_", KeywordKind::UnderscoreLifetime },
				{ "'static", KeywordKind::StaticLifetime },

				// Weak keywords, have special meaning only in specific contexts. ave special meaning only in specific contexts.
				{ "auto", KeywordKind::Auto },
				{ "catch", KeywordKind::Catch },
				{ "default", KeywordKind::Default },
				{ "macro_rules", KeywordKind::MacroRules },
				{ "raw", KeywordKind::Raw },
				{ "union", KeywordKind::Union }
			};

		static inline const std::unordered_map<KeywordKind,std::string> dictionaryKeywordStr
			{
				// Special reserved identifiers used internally for elided lifetimes,
				// unnamed method parameters, crate root module, error recovery etc.
				//{  KeywordKind::PathRoot, "{{root}}" },
				{ KeywordKind::DollarCrate, "$crate" },
				{ KeywordKind::Underscore, "_" },

				// Keywords that are used in stable Rust.
				{ KeywordKind::As, "as" },
				{ KeywordKind::Break, "break" },
				{ KeywordKind::Const, "const" },
				{ KeywordKind::Continue, "continue" },
				{ KeywordKind::Crate, "crate" },
				{ KeywordKind::Else, "else", },
				{ KeywordKind::Enum, "enum", },
				{ KeywordKind::Extern, "extern", },
				{ KeywordKind::False, "false" },
				{ KeywordKind::Fn, "fn" },
				{ KeywordKind::For, "for" },
				{ KeywordKind::If, "if" },
				{ KeywordKind::Impl, "impl" },
				{ KeywordKind::In, "in" },
				{ KeywordKind::Let, "let" },
				{ KeywordKind::Loop, "loop" },
				{ KeywordKind::Match, "match" },
				{ KeywordKind::Mod, "mod" },
				{ KeywordKind::Move, "move" },
				{ KeywordKind::Mut, "mut" },
				{ KeywordKind::Pub, "pub" },
				{ KeywordKind::Ref, "ref" },
				{ KeywordKind::Return, "return" },
				{ KeywordKind::SelfLower, "self" },
				{ KeywordKind::SelfUpper, "Self" },
				{ KeywordKind::Static, "static" },
				{ KeywordKind::Struct, "struct" },
				{ KeywordKind::Super, "super" },
				{ KeywordKind::Trait, "trait" },
				{ KeywordKind::True, "true" },
				{ KeywordKind::Type, "type" },
				{ KeywordKind::Unsafe, "unsafe" },
				{ KeywordKind::Use, "use" },
				{ KeywordKind::Where, "where" },
				{ KeywordKind::While, "while" },

				// Keywords that are used in unstable Rust or reserved for future use.
				{ KeywordKind::Become, "become" },
				{ KeywordKind::Box, "box" },
				{ KeywordKind::Do, "do" },
				{ KeywordKind::Final, "final" },
				{ KeywordKind::Macro, "macro" },
				{ KeywordKind::Override, "override" },
				{ KeywordKind::Priv, "priv" },
				{ KeywordKind::Typeof, "typeof" },
				{ KeywordKind::Unsized, "unsized" },
				{ KeywordKind::Virtual, "virtual" },
				{ KeywordKind::Yield, "yield" },

				// Edition-specific keywords that are used in stable Rust.
				{ KeywordKind::Async, "async" }, // >= 2018 Edition only
				{ KeywordKind::Await, "await" }, // >= 2018 Edition only
				{ KeywordKind::Dyn, "dyn" }, // >= 2018 Edition only

				// Edition-specific keywords that are used in unstable Rust or reserved for future use.
				{ KeywordKind::Try, "try" }, // >= 2018 Edition only

				// Special lifetime names  names
				{ KeywordKind::UnderscoreLifetime, "'_" },
				{ KeywordKind::StaticLifetime, "'static" },

				// Weak keywords, have special meaning only in specific contexts. ave special meaning only in specific contexts.
				{ KeywordKind::Auto, "auto" },
				{ KeywordKind::Catch, "catch" },
				{ KeywordKind::Default, "default" },
				{ KeywordKind::MacroRules, "macro_rules" },
				{ KeywordKind::Raw, "raw" },
				{ KeywordKind::Union, "union" }
			};

		static KeywordKind keywordFromStr(std::string_view view)
		{
			auto res = dictionaryStrKeyword.find(view.data());
			if(res == dictionaryStrKeyword.end())
			{
				return KeywordKind::Invalid;
			}
			return res->second;
		}
		static std::string toString(KeywordKind keyword)
		{
			auto res = dictionaryKeywordStr.find(keyword);
			if(res == dictionaryKeywordStr.end())
			{
				return "";
			}
			return res->second;
		}
		static inline bool isKeyword(KeywordKind kind)
		{
			return kind != KeywordKind::Invalid;
		}
		static inline bool isKeyword(std::string_view view)
		{
			return isKeyword(keywordFromStr(view));
		}
	};
}

#endif //CORROSION_SRC_PARSER_AST_KEYWORD_HPP_
