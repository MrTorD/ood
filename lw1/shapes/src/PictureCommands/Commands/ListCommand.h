#pragma once

#include "ICommand.h"
#include <iostream>

class ListCommand : public ICommand
{
public:
	void Execute(Picture& picture) override
	{
		picture.PrintShapes(std::cout);
	}
};