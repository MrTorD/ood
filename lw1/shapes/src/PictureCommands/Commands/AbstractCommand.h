#pragma once

#include "Picture.h"

class AbstractCommand
{
public:
	virtual void Perform(Picture& picture) = 0;

	virtual ~AbstractCommand() = default;
};
