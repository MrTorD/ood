#pragma once

#include "Picture.h"

class ICommand
{
public:
	virtual void Execute(Picture& picture) = 0;

	virtual ~ICommand() = default;
};
