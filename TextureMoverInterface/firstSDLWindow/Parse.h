#pragma once
#include <string>
#include "Utils.h"

template <class T>
struct Parse { typedef T Type; };

template<>
struct Parse<int> { typedef int Type; };

template<>
struct Parse<bool> { typedef bool Type; };


template<typename T>
T parseImpl(const std::string& data);

template <>
inline int parseImpl<int>(const std::string& data) { return std::stoi(data); }

template <>
inline bool parseImpl<bool>(const std::string& data) { return (data != "0"); }

template <>
inline float parseImpl<float>(const std::string& data) { return std::stof(data); }

template <>
inline double parseImpl<double>(const std::string& data) { return std::stod(data); }

template <>
inline Utils::LDPoint parseImpl<Utils::LDPoint>(const std::string& data)
{ 
	int x = 0, y = 0;

	size_t commaPos = data.find(',');
	if (commaPos != std::string::npos)
	{
		x = std::stoi(data.substr(0, commaPos));
		y = std::stoi(data.substr(commaPos + 1));
	}
	
	return Utils::LDPoint(x, y);
}

template <typename T>
inline T parse(const std::string& data)
{
	return parseImpl<typename Parse<T>::Type>(data);
}

