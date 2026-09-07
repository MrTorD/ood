#pragma once

#include "../Behaviors/DanceBehaviors.h"
#include "../Behaviors/FlyBehaviors.h"
#include "../Behaviors/QuackBehaviors.h"
#include "Duck.h"

class MallardDuck final : public Duck
{
public:
	MallardDuck()
		: Duck(FlyBehaviors::FlyWithWings, QuackBehaviors::LoudQuack, DanceBehaviors::Waltz)
	{
	}

	std::string GetName() const override
	{
		return "MallardDuck";
	}
};