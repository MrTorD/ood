#pragma once

#include <stdexcept>

class NotFoundError : public std::invalid_argument
{
public:
	using std::invalid_argument::invalid_argument;
};