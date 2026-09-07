#pragma once

#include "Duck.h"
#include "FlyBehaviors/FlyWithWings.h"
#include "QuackBehaviors/LoudQuack.h"
#include "DanceBehaviors/Waltz.h"
#include <memory>

class MallardDuck final : public Duck
{
public:
	MallardDuck()
		: Duck(std::make_unique<FlyWithWings>(), std::make_unique<LoudQuack>(), std::make_unique<Waltz>())
	{
	}

	std::string GetName() const override
	{
		return "MallardDuck";
	}
};