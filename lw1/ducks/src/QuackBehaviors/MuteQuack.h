#pragma once

#include "IQuackBehavior.h"
#include <iostream>

class MuteQuack : public IQuackBehavior
{
public:
	void Quack() override
	{
		std::cout << "... someone tries to quack... but it was born in wrong place...\n";
	}
};