#pragma once

#include "DanceNoWay.h"
#include "Duck.h"
#include "FlyNoWay.h"
#include "Squeak.h"
#include <memory>

class RubberDuck final : public Duck
{
public:
	RubberDuck()
		: Duck(std::make_unique<FlyNoWay>(), std::make_unique<Squeak>(), std::make_unique<DanceNoWay>())
	{
	}

	std::string GetName() const override
	{
		return "RubberDuck";
	}
};