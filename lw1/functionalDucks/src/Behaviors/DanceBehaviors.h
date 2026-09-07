#pragma once

#include <iostream>

namespace DanceBehaviors
{

auto DanceNoWay = []() {
	std::cout << "I can't dance. No, I don't want to dance\n";
};

auto Waltz = []() {
	std::cout << "I'm dancing WALTZ\n";
};

auto Minuet = []() {
	std::cout << "I'm dancing MINEUT. Wait, what am I dancing?\n";
};

} // namespace DanceBehaviors