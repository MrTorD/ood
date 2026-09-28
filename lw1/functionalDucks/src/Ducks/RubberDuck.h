#pragma once

#include "DanceBehaviors.h"
#include "FlyBehaviors.h"
#include "QuackBehaviors.h"
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