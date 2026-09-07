#pragma once

#include "../Behaviors/DanceBehaviors.h"
#include "../Behaviors/FlyBehaviors.h"
#include "../Behaviors/QuackBehaviors.h"
#include "Duck.h"

class DecoyDuck final : public Duck
{
public:
	DecoyDuck()
		: Duck(FlyBehaviors::FlyNoWay, QuackBehaviors::MuteQuack, DanceBehaviors::DanceNoWay)
	{
	}

	std::string GetName() const override
	{
		return "DecoyDuck";
	}
};