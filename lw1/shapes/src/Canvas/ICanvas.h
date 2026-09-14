#pragma once

#include "Shapes/Color.h"
#include "Shapes/Point.h"
#include <cstdint>
#include <string>
#include <vector>

class ICanvas
{
public:
	virtual void DrawLine(Point from, Point to, Color color) = 0;
	virtual void DrawPolygon(const std::vector<Point>& points, Color color) = 0;
	virtual void DrawEllipse(Point centre, Point radiuses, Color color) = 0;
	virtual void DrawText(Point topLeft, double fontSize, const std::string& text, Color color) = 0;
	virtual void Flush() = 0;

	virtual ~ICanvas() = default;
};
