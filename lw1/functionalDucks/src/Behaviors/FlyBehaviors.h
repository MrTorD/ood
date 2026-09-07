#pragma once
#include <iostream>

namespace FlyBehaviors
{

auto FlyWithWings = [fliesCount = 0]() mutable {
	std::cout << "I believe I can fly. And I'm flying.\n";
	return ++fliesCount;
};

auto FlyNoWay = []() {
	std::cout << "Born to crawl will never fly...\n";
	return 0;
};

} // namespace FlyBehaviors