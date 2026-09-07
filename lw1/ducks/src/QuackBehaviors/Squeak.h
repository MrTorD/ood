#pragma once

#include "IQuackBehavior.h"
#include <iostream>

class Squeak : public IQuackBehavior
{
public:
	void Quack() override
	{
		std::cout << "Squeak!\n";
	}
};