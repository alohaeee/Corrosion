#ifndef CORROSION_SRC_SPAN_KEYWORDS_AND_SYMS_HPP_
#define CORROSION_SRC_SPAN_KEYWORDS_AND_SYMS_HPP_

namespace corrosion
{
	enum class kw : std::uint32_t
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
	enum class sym : std::uint32_t
	{
		Bool = std::uint32_t(kw::Union) + 1,
		Char,
		Str,
		UInt,
		Int,
		ISize,
		U8,
		I8,
		U16,
		I16,
		U32,
		I32,
		U64,
		I64,
		U128,
		I128,
		F32,
		F64,
		USize,
	};
}




#endif //CORROSION_SRC_SPAN_KEYWORDS_AND_SYMS_HPP_
