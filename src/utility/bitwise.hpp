//
// Created by Vladislav on 01.05.2020.
//

#ifndef CORROSION_SRC_CHAR_BITWISE_HPP_
#define CORROSION_SRC_CHAR_BITWISE_HPP_

#include <type_traits>
#include <stdexcept>
#include <string>

#ifndef BYTE
#define BYTE(num, c) (num >> 8*c & 0xFF)
#endif

#ifndef BIT
#define BIT(num, c) (num >> c & 0x01)
#endif

template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::string toByteStr(T num, std::size_t byte = 0)
{
	std::string str = "0b";
	if (byte < sizeof(T))
	{
		auto b = BYTE(num,byte);
		for (int i = 7; i >= 0; i--)
		{
			str.push_back((BIT(b, i) ? '1' : '0'));
		}
	}
	else
	{
		throw std::invalid_argument("wrong index of byte in num");
	}
	return str;
}

#endif //CORROSION_SRC_CHAR_BITWISE_HPP_
