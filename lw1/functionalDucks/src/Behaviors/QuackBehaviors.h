#pragma once
#include <iostream>

namespace QuackBehaviors
{
    
auto LoudQuack = []() {
	std::cout << "QUACK!!!\n";
};
auto Squeak = []() {
	std::cout << "Squeak!\n";
};
auto MuteQuack = []() {
	std::cout << "... someone tries to quack... but it was born in wrong place...\n";
};

} // namespace QuackBehaviors