#pragma once

#include <stdexcept>

class PictureInvalidOperation : public std::invalid_argument
{
public:
	using std::invalid_argument::invalid_argument;
};