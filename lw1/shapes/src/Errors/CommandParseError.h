#pragma once

#include <stdexcept>

//[ ]: Подумать над типом ошибок
class CommandParseError : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};
