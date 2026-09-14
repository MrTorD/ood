#pragma once

#include <stdexcept>

class PictureInvalidOperation : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};