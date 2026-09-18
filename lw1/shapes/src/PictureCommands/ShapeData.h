#pragma once

#include "Bounds.h"
#include "Color.h"
#include "Point.h"
#include "ShapeType.h"
#include <string>

// [ ]: Попытаться избавиться от CommandData
struct ShapeData
{
	std::string id;

	ShapeType shapeType;

	Bounds bounds;

	Color color;

	unsigned fontSize;
	std::string text;
};