#pragma once

#include "DanceBehaviors/DanceNoWay.h"
#include "Duck.h"
#include "FlyBehaviors/FlyNoWay.h"
#include "QuackBehaviors/Squeak.h"
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