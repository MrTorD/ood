#pragma once

#include "Duck.h"
#include "FlyWithWings.h"
#include "LoudQuack.h"
#include "Waltz.h"
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