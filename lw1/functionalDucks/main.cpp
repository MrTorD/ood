#include "../ducks/PlayWithDuck.h"
#include "../functionalDucks/src/Ducks/DecoyDuck.h"
#include "../functionalDucks/src/Ducks/MallardDuck.h"
#include "../functionalDucks/src/Ducks/ReadHeadDuck.h"
#include "../functionalDucks/src/Ducks/RubberDuck.h"
#include <iostream>
#include <vector>

int main()
{
	std::vector<std::unique_ptr<Duck>> ducks;

	ducks.push_back(std::make_unique<MallardDuck>());
	ducks.push_back(std::make_unique<DecoyDuck>());
	ducks.push_back(std::make_unique<ReadHeadDuck>());
	ducks.push_back(std::make_unique<RubberDuck>());

	for (auto&& duck : ducks)
	{
		PlayWithDuck(std::move(duck));
	}
}
