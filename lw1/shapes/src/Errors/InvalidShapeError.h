#pragma once

#include <stdexcept>

class InvalidShapeError : public std::invalid_argument
{
public:
	using std::invalid_argument::invalid_argument;
};