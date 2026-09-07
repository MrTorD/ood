#pragma once

#include "../Behaviors/DanceBehaviors.h"
#include "../Behaviors/FlyBehaviors.h"
#include "../Behaviors/QuackBehaviors.h"
#include "Duck.h"

class ReadHeadDuck final : public Duck
{
public:
	ReadHeadDuck()
		: Duck(FlyBehaviors::FlyWithWings, QuackBehaviors::LoudQuack, DanceBehaviors::Minuet)
	{
	}

	std::string GetName() const override
	{
		return "ReadHeadDuck";
	}
};