#pragma once

#include "../Shapes/Color.h"
#include "../Shapes/Point.h"
#include "../Shapes/Rect.h"
#include <string>
#include "ShapeType.h"

struct CommandData
{
	std::string id;

	ShapeType shapeType;

	Rect bounds;

	Color color;

	unsigned fontSize;
	std::string text;
};