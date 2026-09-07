#pragma once

#include "IDanceBehavior.h"
#include <iostream>

class DanceNoWay : public IDanceBehavior
{
public:
	void Dance() override
	{
		std::cout << "I can't dance. No, I don't want to dance\n";
	}
};
