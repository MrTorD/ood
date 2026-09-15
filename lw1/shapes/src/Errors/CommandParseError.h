#pragma once

#include <stdexcept>

//TODO: подумать над типом ошибок
class CommandParseError : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};
