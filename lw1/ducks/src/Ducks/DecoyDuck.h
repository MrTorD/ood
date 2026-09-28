#pragma once

#include "DanceNoWay.h"
#include "Duck.h"
#include "FlyNoWay.h"
#include "MuteQuack.h"
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