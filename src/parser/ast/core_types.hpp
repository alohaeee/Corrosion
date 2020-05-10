#ifndef CORROSION_SRC_PARSER_AST_CORE_TYPES_HPP_
#define CORROSION_SRC_PARSER_AST_CORE_TYPES_HPP_

#include "utility/fwd.hpp"

namespace corrosion
{
	/**
	 * @brief Enumeration of core types.
	 * Must be sorted.
	 */
	enum class TypeKind
	{
		Invalid,
		Any,
		Bool,
		Char, Str,
		UInt, Int,
		U8, I8,
		U16, I16,
		U32, I32,
		U64, I64,
		U128, I128,
		F32,
		F64,
	};
	struct CoreTypes
	{
		static inline const std::unordered_map<std::string, TypeKind> dictionary
			{
				{"_", TypeKind::Any},
				{"bool", TypeKind::Bool},
				{"char", TypeKind::Char},
				{"f32", TypeKind::F32},
				{"f64", TypeKind::F64},
				{"i128", TypeKind::I128},
				{"i16", TypeKind::I16},
				{"i32", TypeKind::I32},
				{"i64", TypeKind::I64},
				{"i8", TypeKind::I8},
				{"int", TypeKind::Int},
				{"isize", TypeKind::Int},
				{"str", TypeKind::Str},
				{"u128", TypeKind::U128},
				{"u16", TypeKind::U16},
				{"u32", TypeKind::U32},
				{"u64", TypeKind::U64},
				{"u8",  TypeKind::U8},
				{"uint", TypeKind::UInt},
				{"usize", TypeKind::UInt},
			};
		static TypeKind typeFromStr(std::string_view view)
		{
			auto res = dictionary.find(view.data());
			if(res == dictionary.end())
			{
				return TypeKind::Invalid;
			}
			return res->second;
		}
		static std::string toString(TypeKind type)
		{
			switch(type)
			{
			case TypeKind::Invalid:
				return "Invalid";
			case TypeKind::Any:
				return "_";
			case TypeKind::Bool:
				return "bool";
			case  TypeKind::Char:
				return "char";
			case TypeKind::F32:
				return "f32";
			case TypeKind::F64:
				return "f64";
			case  TypeKind::I128:
				return "i128";
			case TypeKind::I16:
				return "i16";
			case TypeKind::I32:
				return "i32";
			case TypeKind::I64:
				return "i64";
			case TypeKind::I8:
				return "i8";
			case TypeKind::Int:
				return "int(isize)";
			case TypeKind::Str:
				return "str";
			case TypeKind::U128:
				return "u128";
			case TypeKind::U16:
				return "u16";
			case TypeKind::U32:
				return "u32";
			case TypeKind::U64:
				return "u64";
			case TypeKind::U8:
				return "u8";
			case TypeKind::UInt:
				return "uint(usize)";
			}

		}
		static inline bool canBeSuffix(TypeKind type)
		{
			return TypeKind::UInt >= type && type <= TypeKind::F64;
		}
		static inline bool canBeSuffix(std::string_view view)
		{
			return canBeSuffix(typeFromStr(view));
		}

		static inline bool isType(TypeKind type)
		{
			return type != TypeKind::Invalid;
		}
		static inline bool isType(std::string_view view)
		{
			return isType(typeFromStr(view));
		}
	};

}

#endif //CORROSION_SRC_PARSER_AST_CORE_TYPES_HPP_
