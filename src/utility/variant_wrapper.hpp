#ifndef CORROSION_SRC_UTILITY_VARIANT_WRAPPER_HPP_
#define CORROSION_SRC_UTILITY_VARIANT_WRAPPER_HPP_

template<typename ...Types>
struct VariantWrapper : public std::variant<Types...>
{
	template<typename T>
	inline const T& get() const
	{
		return std::get<T>(*this);
	}
	template<typename T>
	inline T& get()
	{
		return std::get<T>(*this);
	}
	template<typename T>
	inline auto getIf() noexcept
	{
		return std::get_if<T>(this);
	}
	template<typename T>
	inline auto getIf() const noexcept
	{
		return std::get_if<T>(this);
	}
	template<typename T>
	inline auto getOpt() noexcept
	{
		if (auto ptr = this->get_if<T>(); ptr)
		{
			return std::optional<T>(*ptr);
		}
		return std::nullopt;

	}
	inline auto& toVariant() const
	{
		return static_cast<std::variant<Types...>>(*this);
	}
	template<typename T>
	inline bool holds() const noexcept
	{
		return std::holds_alternative<T>(*this);
	};

};

#endif //CORROSION_SRC_UTILITY_VARIANT_WRAPPER_HPP_
