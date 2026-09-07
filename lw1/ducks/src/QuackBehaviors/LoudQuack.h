#pragma once

#include "IQuackBehavior.h"
#include <iostream>

class LoudQuack : public IQuackBehavior
{
public:
	void Quack() override
	{
		std::cout << "QUACK!!!\n";
	}
};