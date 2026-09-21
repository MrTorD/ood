#pragma once

#include <stdexcept>

//[x]: Подумать над типом ошибок
class CommandParseError : public std::invalid_argument
{
public:
	using std::invalid_argument::invalid_argument;
};
