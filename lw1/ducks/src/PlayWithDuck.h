#pragma once

#include "Duck.h"
#include <memory>

void PlayWithDuck(std::unique_ptr<Duck> duck)
{
	std::cout << "Hi, I'm " << duck->GetName() << "\n";
	duck->Quack();
	duck->Dance();
	duck->Fly();
	duck->Swim();
	std::cout << "Bye!\n";
}