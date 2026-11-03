#pragma once

#include "Point.h"
#include <SFML/System/Vector2.hpp>

Point ConvertToPoint(sf::Vector2i vec)
{
	return { .x = static_cast<float>(vec.x), .y = static_cast<float>(vec.y) };
}

Point ConvertToPoint(sf::Vector2f vec)
{
	return { .x = vec.x, .y = vec.y };
}

sf::Vector2f ConvertToVector(Point point)
{
	return { point.x, point.y };
}