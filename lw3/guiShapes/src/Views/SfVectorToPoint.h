#pragma once

#include "Point.h"
#include <SFML/System/Vector2.hpp>

Point ConvertToPoint(sf::Vector2i vec)
{
	return { (float)vec.x, (float)vec.y };
}