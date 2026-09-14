#pragma once

#include "AbstractCommand.h"
#include <iostream>

class ListCommand : public AbstractCommand
{
public:
	void Perform(Picture& picture) override
	{
		picture.PrintShapes(std::cout);
	}
};