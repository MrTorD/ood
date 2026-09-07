#pragma once

#include "DanceBehaviors/DanceNoWay.h"
#include "Duck.h"
#include "FlyBehaviors/FlyNoWay.h"
#include "QuackBehaviors/MuteQuack.h"
#include <memory>

class DecoyDuck final : public Duck
{
public:
	DecoyDuck()
		: Duck(std::make_unique<FlyNoWay>(), std::make_unique<MuteQuack>(), std::make_unique<DanceNoWay>())
	{
	}

	std::string GetName() const override
	{
		return "DecoyDuck";
	}
};