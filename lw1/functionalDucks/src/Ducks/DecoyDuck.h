#pragma once

#include "DanceBehaviors.h"
#include "FlyBehaviors.h"
#include "QuackBehaviors.h"
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