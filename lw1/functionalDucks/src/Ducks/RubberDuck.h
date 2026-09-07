#pragma once

#include "../Behaviors/DanceBehaviors.h"
#include "../Behaviors/FlyBehaviors.h"
#include "../Behaviors/QuackBehaviors.h"
#include "Duck.h"

class RubberDuck final : public Duck
{
public:
	RubberDuck()
		: Duck(FlyBehaviors::FlyNoWay, QuackBehaviors::Squeak, DanceBehaviors::DanceNoWay)
	{
	}

	std::string GetName() const override
	{
		return "RubberDuck";
	}
};