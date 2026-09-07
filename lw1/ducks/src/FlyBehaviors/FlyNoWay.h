#pragma once

#include "IFlyBehavior.h"
#include <iostream>

class FlyNoWay : public IFlyBehavior
{
public:
	void Fly() override
	{
		std::cout << "Born to crawl will never fly...\n";
	}

	unsigned GetFliesCount() override
	{
		return 0;
	}
};