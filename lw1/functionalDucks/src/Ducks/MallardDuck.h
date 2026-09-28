#pragma once

#include "DanceBehaviors.h"
#include "FlyBehaviors.h"
#include "QuackBehaviors.h"
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