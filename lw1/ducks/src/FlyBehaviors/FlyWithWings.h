#pragma once

#include "IFlyBehavior.h"
#include <iostream>

class FlyWithWings : public IFlyBehavior
{
public:
	void Fly() override
	{
		std::cout << "I believe I can fly. And I'm flying.\n";
		m_fliesCount++;
	}

	bool IsFlyable() override
	{
		return true;
	}

	unsigned GetFliesCount() override
	{
		return m_fliesCount;
	}

private:
	unsigned m_fliesCount = 0;
};