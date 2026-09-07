#pragma once

#include "IDanceBehavior.h"
#include <iostream>

class Waltz : public IDanceBehavior
{
public:
	void Dance() override
	{
        std::cout << "I'm dancing WALTZ\n";
	}
};
