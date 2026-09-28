#pragma once

#include "DanceBehaviors.h"
#include "FlyBehaviors.h"
#include "QuackBehaviors.h"
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