#pragma once

#include "Duck.h"
#include "FlyWithWings.h"
#include "LoudQuack.h"
#include "Minuet.h"
#include <memory>

class ReadHeadDuck final : public Duck
{
public:
	ReadHeadDuck()
		: Duck(std::make_unique<FlyWithWings>(), std::make_unique<LoudQuack>(), std::make_unique<Minuet>())
	{
	}

	std::string GetName() const override
	{
		return "ReadHeadDuck";
	}
};